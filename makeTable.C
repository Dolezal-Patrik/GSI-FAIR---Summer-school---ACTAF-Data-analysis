// makeTable.C
// ---------------------------------------------------------------------------
// Builds an ASCII table (Pad, Mu, MuErr, Sigma, SigmaErr) from the per-run
// *_fit_results.txt files and adds the run conditions (pressure, HV, beam,
// comment) taken from a small logbook file (runlog.txt).
//
// Usage (from the shell / WSL):
//   root -l 'makeTable.C("alpha_histograms_nearline_data_2362_20260827_141318_fit_results.txt")'
//   root -l 'makeTable.C("FITDATA")'                      // whole folder, one output file
//   root -l 'makeTable.C("FITDATA", "runlog.txt", "my_tables.txt")'
//
// Arguments:
//   input    - one fit-results txt file OR a folder (all *_fit_results.txt inside)
//   logbook  - logbook file, default "runlog.txt"; looked up in the current
//              folder first, then in the folder of the input file / input folder
//   outFile  - output txt file; default: <input>_table.txt  or  <folder>/all_runs_table.txt
//
// The run number is taken from the file name (the number after "_data_").
//
// Logbook format (runlog.txt), whitespace separated, '#' = comment line:
//   Run  P[bar]  HV[kV]  V[V]  Beam(y/n)  Comment...
//   V may be "-"; then it is computed as 75 V per kV and marked with '*'.
//
// Plain C++ only (no ROOT classes), so it also compiles with g++.
// ---------------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>

namespace MakeTableImpl {

const double kVoltPerKV   = 75.0;  // observed ratio V / kV in the logbook
const double kNominalKVpB = 5.0;   // nominal HV = 5 kV per bar of pressure

struct RunInfo {
  int         run      = -1;
  double      pressure = -1;   // [bar]
  double      hvKV     = -1;   // [kV]
  double      hvV      = -1;   // [V], -1 = not given in the logbook
  std::string beam     = "?";  // y / n
  std::string comment;
  bool        known    = false;
};

struct PadRow {
  int    pad = 0;
  double entries = 0, mu = 0, muErr = 0, sigma = 0, sigmaErr = 0, chi2ndf = 0;
};

std::string trim(const std::string& s) {
  size_t a = s.find_first_not_of(" \t\r\n");
  if (a == std::string::npos) return "";
  size_t b = s.find_last_not_of(" \t\r\n");
  return s.substr(a, b - a + 1);
}

bool endsWith(const std::string& s, const std::string& suffix) {
  return s.size() >= suffix.size() &&
         s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool isDirectory(const std::string& path) {
  struct stat st;
  return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

bool fileExists(const std::string& path) {
  struct stat st;
  return stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

std::string dirName(const std::string& path) {
  size_t p = path.find_last_of("/\\");
  return (p == std::string::npos) ? "." : path.substr(0, p);
}

std::string baseName(const std::string& path) {
  size_t p = path.find_last_of("/\\");
  return (p == std::string::npos) ? path : path.substr(p + 1);
}

// Run number = digits right after "_data_" in the file name (-1 if not found)
int runFromFileName(const std::string& path) {
  std::string name = baseName(path);
  size_t p = name.find("_data_");
  if (p == std::string::npos) return -1;
  p += 6;
  size_t q = p;
  while (q < name.size() && isdigit((unsigned char)name[q])) ++q;
  if (q == p) return -1;
  return atoi(name.substr(p, q - p).c_str());
}

std::map<int, RunInfo> readLogbook(const std::string& path) {
  std::map<int, RunInfo> book;
  std::ifstream in(path.c_str());
  if (!in) {
    std::cerr << "WARNING: cannot open logbook '" << path
              << "' - run conditions will be missing.\n";
    return book;
  }
  std::string line;
  while (std::getline(in, line)) {
    line = trim(line);
    if (line.empty() || line[0] == '#') continue;
    std::istringstream ss(line);
    RunInfo r;
    std::string vStr;
    if (!(ss >> r.run >> r.pressure >> r.hvKV >> vStr >> r.beam)) {
      std::cerr << "WARNING: skipping unparsable logbook line: " << line << "\n";
      continue;
    }
    r.hvV = (vStr == "-") ? -1 : atof(vStr.c_str());
    std::getline(ss, r.comment);
    r.comment = trim(r.comment);
    r.known = true;
    book[r.run] = r;
  }
  return book;
}

std::vector<PadRow> readFitFile(const std::string& path) {
  std::vector<PadRow> rows;
  std::ifstream in(path.c_str());
  if (!in) {
    std::cerr << "ERROR: cannot open fit file '" << path << "'\n";
    return rows;
  }
  std::string line;
  while (std::getline(in, line)) {
    line = trim(line);
    if (line.empty() || line[0] == '#' || line.compare(0, 3, "Pad") == 0) continue;
    std::istringstream ss(line);
    PadRow r;
    if (ss >> r.pad >> r.entries >> r.mu >> r.muErr >> r.sigma >> r.sigmaErr >> r.chi2ndf)
      rows.push_back(r);
  }
  return rows;
}

std::string num(double v, int prec) {
  char buf[64];
  snprintf(buf, sizeof(buf), "%.*f", prec, v);
  return buf;
}

std::string cell(const std::string& s, size_t w) {  // right-aligned cell
  std::string t = s;
  if (t.size() < w) t = std::string(w - t.size(), ' ') + t;
  return " " + t + " ";
}

std::string headCell(const std::string& s, size_t w) {  // centred header cell
  size_t left = (w > s.size()) ? (w - s.size()) / 2 : 0;
  size_t right = (w > s.size()) ? w - s.size() - left : 0;
  return " " + std::string(left, ' ') + s + std::string(right, ' ') + " ";
}

// One block = run header + table
std::string buildBlock(const std::string& file, int run, const RunInfo& info,
                       const std::vector<PadRow>& rows) {
  std::ostringstream out;

  out << "Run " << run;
  if (info.known) {
    double nominal = info.hvKV / (kNominalKVpB * info.pressure) * 100.0;
    bool   vGuess  = (info.hvV < 0);
    double v       = vGuess ? info.hvKV * kVoltPerKV : info.hvV;
    out << "  |  P = " << num(info.pressure, 0) << " bar"
        << "  |  HV = " << num(info.hvKV, 0) << " kV"
        << "  |  V = " << num(v, 0) << " V" << (vGuess ? "*" : "")
        << "  |  " << num(nominal, 0) << " % nominal"
        << "  |  beam: " << info.beam;
    if (!info.comment.empty()) out << "  |  " << info.comment;
  } else {
    out << "  |  (not in logbook - add it to runlog.txt)";
  }
  out << "\nFile: " << baseName(file) << "\n";

  // column widths
  const size_t wPad = 3, wVal = 9, wErr = 8;
  std::vector<std::string> pad, mu, muE, sg, sgE;
  size_t wp = wPad, wm = wVal, wme = wErr, ws = wVal, wse = wErr;
  for (size_t i = 0; i < rows.size(); ++i) {
    pad.push_back(num(rows[i].pad, 0));
    mu.push_back(num(rows[i].mu, 3));
    muE.push_back(num(rows[i].muErr, 3));
    sg.push_back(num(rows[i].sigma, 3));
    sgE.push_back(num(rows[i].sigmaErr, 3));
    wp  = std::max(wp,  pad[i].size());
    wm  = std::max(wm,  mu[i].size());
    wme = std::max(wme, muE[i].size());
    ws  = std::max(ws,  sg[i].size());
    wse = std::max(wse, sgE[i].size());
  }

  std::string sep = "+" + std::string(wp + 2, '-') + "+" + std::string(wm + 2, '-') +
                    "+" + std::string(wme + 2, '-') + "+" + std::string(ws + 2, '-') +
                    "+" + std::string(wse + 2, '-') + "+\n";

  out << sep
      << "|" << headCell("Pad", wp) << "|" << headCell("Mu", wm) << "|"
      << headCell("MuErr", wme) << "|" << headCell("Sigma", ws) << "|"
      << headCell("SigmaErr", wse) << "|\n"
      << sep;
  for (size_t i = 0; i < rows.size(); ++i) {
    out << "|" << cell(pad[i], wp) << "|" << cell(mu[i], wm) << "|"
        << cell(muE[i], wme) << "|" << cell(sg[i], ws) << "|"
        << cell(sgE[i], wse) << "|\n";
  }
  out << sep;
  return out.str();
}

std::vector<std::string> listFitFiles(const std::string& dir) {
  std::vector<std::string> files;
  DIR* d = opendir(dir.c_str());
  if (!d) return files;
  while (struct dirent* e = readdir(d)) {
    std::string n = e->d_name;
    if (endsWith(n, "_fit_results.txt")) files.push_back(dir + "/" + n);
  }
  closedir(d);
  // sort by run number, then by name
  std::sort(files.begin(), files.end(), [](const std::string& a, const std::string& b) {
    int ra = runFromFileName(a), rb = runFromFileName(b);
    return (ra != rb) ? ra < rb : a < b;
  });
  return files;
}

}  // namespace MakeTableImpl

void makeTable(const char* input, const char* logbook = "runlog.txt",
               const char* outFile = "") {
  using namespace MakeTableImpl;

  std::string in(input);
  while (in.size() > 1 && in[in.size() - 1] == '/') in.erase(in.size() - 1);

  std::vector<std::string> files;
  std::string out(outFile);

  if (isDirectory(in)) {
    files = listFitFiles(in);
    if (out.empty()) out = in + "/all_runs_table.txt";
  } else {
    files.push_back(in);
    if (out.empty()) {
      out = in;
      if (endsWith(out, ".txt")) out.erase(out.size() - 4);
      out += "_table.txt";
    }
  }
  if (files.empty()) {
    std::cerr << "ERROR: no *_fit_results.txt files found in '" << in << "'\n";
    return;
  }

  // Logbook lookup: 1) path as given (relative to the folder where root was
  // started), 2) the same file name next to the input file / inside the input folder
  std::string lb(logbook);
  if (!fileExists(lb)) {
    std::string alt = (isDirectory(in) ? in : dirName(in)) + "/" + baseName(lb);
    if (fileExists(alt)) lb = alt;
  }
  std::cout << "Using logbook: " << lb << "\n";
  std::map<int, RunInfo> book = readLogbook(lb);

  std::ostringstream all;
  bool anyGuess = false;
  for (size_t i = 0; i < files.size(); ++i) {
    int run = runFromFileName(files[i]);
    std::vector<PadRow> rows = readFitFile(files[i]);
    if (rows.empty()) {
      std::cerr << "WARNING: no pad rows read from " << files[i] << "\n";
      continue;
    }
    RunInfo info;
    std::map<int, RunInfo>::const_iterator it = book.find(run);
    if (it != book.end()) {
      info = it->second;
      if (info.hvV < 0) anyGuess = true;
    } else {
      std::cerr << "WARNING: run " << run << " not found in logbook '" << logbook << "'\n";
    }
    all << buildBlock(files[i], run, info, rows) << "\n";
  }
  if (anyGuess)
    all << "* V not given in the logbook, computed as 75 V per kV of HV\n";

  std::cout << all.str();

  std::ofstream f(out.c_str());
  if (f) {
    f << all.str();
    std::cout << "Table written to: " << out << "\n";
  } else {
    std::cerr << "WARNING: cannot write output file '" << out << "'\n";
  }
}

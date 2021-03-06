
#include <stdlib.h>
#include <string>
#include <vector>
#include <signal.h>
#include <iostream>

#include "SickS300.hpp"
#include "generic-laser-scanner/applications/LaserScannerTools.hpp"




using namespace std;
using namespace brics_oodl;




int main(int argc, char * argv[]) {

  (Logger::getInstance()).init();

  LaserScannerConfiguration config;
  config.devicePath = "/dev/ttyUSB0"; // Device path of the Sick S300
  config.baud = BAUD_500K;

  SickS300 scanner;

  Errors errors;

  if (!scanner.setConfiguration(config, errors)) {
    errors.printErrorsToConsole();
    return -1;
  }


  LaserScannerTools tools;

  tools.plot_laserscanner_values(scanner, errors);
  

  /*
   * Uninitialize the device
   */
  try {
    scanner.close(errors);
  } catch (...) {
    cerr << "Uninitialize failed!" << endl;
    return -1;
  }

  /* Success! */
  return 0;

}

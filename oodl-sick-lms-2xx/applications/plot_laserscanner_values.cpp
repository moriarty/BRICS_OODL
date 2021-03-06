
#include <stdlib.h>
#include <string>
#include <vector>
#include <signal.h>
#include <iostream>

#include "generic-laser-scanner/LaserScannerConfiguration.hpp"
#include "SickLMS2xxConfiguration.hpp"
#include "generic-laser-scanner/LaserScannerData.hpp"
#include "generic-laser-scanner/LaserScannerDataWithIntensities.hpp"
#include "generic-laser-scanner/LaserScanner.hpp"
#include "SickLMS2xx.hpp"
#include "generic/Errors.hpp"
#include "generic/Units.hpp"
#include "generic/Logger.hpp"



using namespace std;
using namespace brics_oodl;




int main(int argc, char * argv[]) {


  (Logger::getInstance()).init();
  

  SickLMS2xxConfiguration config;
  config.devicePath = "/dev/ttyUSB0"; // Device path of the Sick LMS 2xx
  config.baud = BAUD_500K;

  SickLMS2xx scanner;

  Errors errors;

  if (!scanner.setConfiguration(config, errors)) {
    errors.printErrorsToConsole();
    return -1;
  }


 
  

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

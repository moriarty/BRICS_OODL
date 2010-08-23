#ifndef _SICKS300CONFIGURATION_H
#define _SICKS300CONFIGURATION_H


#include "LaserScannerConfiguration.h"

class SickS300Configuration : public LaserScannerConfiguration {
  public:
    SickS300Configuration();

    virtual ~SickS300Configuration();

    SickS300Configuration(const SickS300Configuration & source);

    virtual SickS300Configuration & operator=(const SickS300Configuration & source);

};
#endif
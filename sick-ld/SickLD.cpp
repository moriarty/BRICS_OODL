/**
 * \file 
 *
 * \author
 * \date
 */
#include "SickLD.h"
#include "Errors.h"
#include "LaserScannerConfiguration.h"
#include "SickLMS2xxConfiguration.h"
#include "LaserScannerData.h"
#include "LaserScannerDataWithIntensities.h"

SickLD::SickLD() {
  // Bouml preserved body begin 00026771
  this->sick_lms = NULL;
  this->config = NULL;
  this->isConnected = false;
  // Bouml preserved body end 00026771
}

SickLD::~SickLD() {
  // Bouml preserved body begin 000267F1
  Errors er;
  this->close(er);
  if (this->config != NULL) {
    delete this->config;
  }
  // Bouml preserved body end 000267F1
}

bool SickLD::close(Errors& error) {
  // Bouml preserved body begin 00026871
  if (this->sick_lms != NULL) {
    try {
      this->sick_lms->Uninitialize();
    } catch (...) {
      error.addError("unable_to_uninitialize", "could not uninitialize the Sick LMS");
      return false;
    }
    delete sick_lms;
    this->sick_lms = NULL;
  }
  this->isConnected = false;
  // Bouml preserved body end 00026871
}

bool SickLD::setConfiguration(const LaserScannerConfiguration& configuration, Errors& error) {
  // Bouml preserved body begin 000268F1
  error.addError("configuration_not_possible", "the configuration is not possible. Please configurate with SickLMS2xxConfiguration.");
  /*
  if (this->config != NULL) {
    delete this->config;
  }
  this->config = new LaserScannerConfiguration;
   *(this->config) = configuration;
  
  if (!this->open(error)) {
    return false;
  }
   */
  // Bouml preserved body end 000268F1
}

bool SickLD::setConfiguration(const SickLMS2xxConfiguration& configuration, Errors& error) {
  // Bouml preserved body begin 00026971
  if (this->config != NULL) {
    delete this->config;
  }
  this->config = new SickLMS2xxConfiguration;
  *(this->config) = configuration;

  if (!this->open(error)) {
    return false;
  }

  SickLMS2xxConfiguration currentConfig;
  if (!this->getConfiguration(currentConfig, error)) {
    return false;
  }

  try {

    if (configuration.availability_level != currentConfig.availability_level) {
      this->sick_lms->SetSickAvailability(configuration.availability_level);
    }
    if (configuration.measuring_mode != currentConfig.measuring_mode) {
      this->sick_lms->SetSickMeasuringMode(configuration.measuring_mode);
    }
    if (configuration.sensitivity != currentConfig.sensitivity) {
      this->sick_lms->SetSickSensitivity(configuration.sensitivity);
    }
    if (configuration.peak_threshold != currentConfig.peak_threshold) {
      this->sick_lms->SetSickPeakThreshold(configuration.peak_threshold);
    }
    if (configuration.measuring_units != currentConfig.measuring_units) {
      this->sick_lms->SetSickMeasuringUnits(configuration.measuring_units);
    }
    if ((configuration.scan_angle != currentConfig.scan_angle) || (configuration.scan_resolution != currentConfig.scan_resolution)) {

      sick_lms_scan_angle_t desired_scan_angle;
      sick_lms_scan_resolution_t desired_scan_resolution;

      double angle = configuration.scan_angle.value()*180.0 / M_PI;
      double resolution = configuration.scan_resolution.value()*180.0 / M_PI;
      double offset = 1;

      if (90 > angle - offset && 90 < angle + offset) {
        desired_scan_angle = SickLMS::SICK_SCAN_ANGLE_90;

      } else if (100 > angle - offset && 100 < angle + offset) {
        desired_scan_angle = SickLMS::SICK_SCAN_ANGLE_100;

      } else if (180 > angle - offset && 180 < angle + offset) {
        desired_scan_angle = SickLMS::SICK_SCAN_ANGLE_180;

      } else {
        desired_scan_angle = SickLMS::SICK_SCAN_ANGLE_UNKNOWN;
        error.addError("unknown_scan_angle", "the scan angle is unknown. Please select one of 90°, 100° or 180°");
      }


      if (25 > resolution - offset && 25 < resolution + offset) {
        desired_scan_resolution = SickLMS::SICK_SCAN_RESOLUTION_25;

      } else if (50 > resolution - offset && 50 < resolution + offset) {
        desired_scan_resolution = SickLMS::SICK_SCAN_RESOLUTION_50;

      } else if (100 > resolution - offset && 100 < resolution + offset) {
        desired_scan_resolution = SickLMS::SICK_SCAN_RESOLUTION_100;

      } else {
        desired_scan_resolution = SickLMS::SICK_SCAN_RESOLUTION_UNKNOWN;
        error.addError("unknown_scan_resolution", "the scan resolution is unknown. Please select one of 0.25°, 0.5° or 1°");
      }

      this->sick_lms->SetSickVariant(desired_scan_angle, desired_scan_resolution);
    }
  } catch (...) {
    error.addError("unable_to_set_configuration", "could not set the configuration to the Sick LMS");
    return false;
  }

  // configuration.operating_mode = this->sick_lms->GetSickOperatingMode();
  // configuration.model = this->sick_lms->SickTypeToString(this->sick_lms->GetSickType());

  // Bouml preserved body end 00026971
}

bool SickLD::getConfiguration(LaserScannerConfiguration& configuration, Errors& error) {
  // Bouml preserved body begin 000269F1
  if (!this->open(error)) {
    return false;
  }
  try {
    configuration.vendor = "SICK";
    configuration.product = "LMS";
    configuration.measuring_mode = this->sick_lms->SickMeasuringModeToString(this->sick_lms->GetSickMeasuringMode());
    configuration.sensitivity = this->sick_lms->SickSensitivityToString(this->sick_lms->GetSickSensitivity());
    configuration.peak_threshold = this->sick_lms->SickPeakThresholdToString(this->sick_lms->GetSickPeakThreshold());
    configuration.scan_angle = this->sick_lms->GetSickScanAngle() * M_PI / 180.0 * radian;
    configuration.scan_resolution = this->sick_lms->GetSickScanResolution() * M_PI / 180.0 * radian;
    configuration.firmware = this->sick_lms->GetSickSoftwareVersionAsString();
    // this->sick_lms->GetSickStatusAsString();
    configuration.operating_mode = this->sick_lms->SickOperatingModeToString(this->sick_lms->GetSickOperatingMode());
    configuration.model = this->sick_lms->SickTypeToString(this->sick_lms->GetSickType());

  } catch (...) {
    error.addError("unable_to_read_configuration", "could not get the configuration from the Sick LMS");
    return false;
  }

  return true;
  // Bouml preserved body end 000269F1
}

bool SickLD::getConfiguration(SickLMS2xxConfiguration& configuration, Errors& error) {
  // Bouml preserved body begin 00026A71
  if (!this->open(error)) {
    return false;
  }

  try {
    configuration.vendor = "SICK";
    configuration.product = "LMS";
    configuration.availability_level = this->sick_lms->GetSickAvailability();
    configuration.measuring_mode = this->sick_lms->GetSickMeasuringMode();
    configuration.sensitivity = this->sick_lms->GetSickSensitivity();
    configuration.peak_threshold = this->sick_lms->GetSickPeakThreshold();
    configuration.scan_angle = this->sick_lms->GetSickScanAngle() * M_PI / 180.0 * radian;
    configuration.scan_resolution = this->sick_lms->GetSickScanResolution() * M_PI / 180.0 * radian;
    configuration.firmware = this->sick_lms->GetSickSoftwareVersionAsString();
    // this->sick_lms->GetSickStatusAsString();
    configuration.operating_mode = this->sick_lms->GetSickOperatingMode();
    configuration.measuring_units = this->sick_lms->GetSickMeasuringUnits();
    configuration.model = this->sick_lms->SickTypeToString(this->sick_lms->GetSickType());
    configuration.is_sick_lms_fast = this->sick_lms->IsSickLMSFast();

  } catch (...) {
    error.addError("unable_to_read_configuration", "could not get the configuration from the Sick LMS");
    return false;
  }

  return true;
  // Bouml preserved body end 00026A71
}

bool SickLD::getData(LaserScannerData& data, Errors& error) {
  // Bouml preserved body begin 00026AF1
  if (!this->open(error)) {
    return false;
  }
  try {
    unsigned int size = data.getNumMeasurementValues();
    
    //Gets measurement data from the Sick. NOTE: Data can be either range or reflectivity given the Sick mode.
    this->sick_lms->GetSickScan(data.getRangesPointer(), size);

  } catch (...) {
    error.addError("unable_to_get_data", "could not get data from the Sick LMS");
    return false;
  }

  return true;
  // Bouml preserved body end 00026AF1
}

bool SickLD::getData(LaserScannerDataWithIntensities& data, Errors& error) {
  // Bouml preserved body begin 00026B71
   if (!this->open(error)) {
    return false;
  }
  try {
    unsigned int ranges_size = data.getNumMeasurementValues();
    unsigned int intensities_size = data.getNumIntensitiesValues();

    //Gets range and reflectivity data from the Sick. NOTE: This only applies to Sick LMS 211/221/291-S14!
    this->sick_lms->GetSickScan(data.getRangesPointer(), data.getIntensitiesPointer(), ranges_size, intensities_size);

  } catch (...) {
    error.addError("unable_to_get_data", "could not get data from the Sick LMS");
    return false;
  }

  return true;
  // Bouml preserved body end 00026B71
}

bool SickLD::resetDevice() {
  // Bouml preserved body begin 00026BF1
  Errors error;
  if (!this->open(error)) {
    return false;
  }
  try {
    this->sick_lms->ResetSick();
  } catch (...) {
    error.addError("unable_to_reset_Sick_LMS", "could not reset the Sick LMS");
    return false;
  }
  return true;
  // Bouml preserved body end 00026BF1
}

bool SickLD::open(Errors& error) {
  // Bouml preserved body begin 00026C71
  if (this->isConnected) {
    return true;
  }

  if (this->config->device_path == "") {
    error.addError("no_DevicePath", "the device path is not specified in the configuration");
    this->isConnected = false;
    return false;
  }

  if (this->sick_lms != NULL) {
    error.addError("still_Connected", "a previous connection was not closed correctly please close it again.");
    this->isConnected = false;
    return false;
  }

  this->sick_lms = new SickLMS(this->config->device_path);

  SickLMS::sick_lms_baud_t desired_baud = SickLMS::SICK_BAUD_38400;

  switch (this->config->boud) {
    case BAUD_9600:
      desired_baud = SickLMS::SICK_BAUD_9600;
      break;
    case BAUD_19200:
      desired_baud = SickLMS::SICK_BAUD_19200;
      break;
    case BAUD_38400:
      desired_baud = SickLMS::SICK_BAUD_38400;
      break;
    case BAUD_500K:
      desired_baud = SickLMS::SICK_BAUD_500K;
      break;
    case BAUD_UNKNOWN:
      desired_baud = SickLMS::SICK_BAUD_UNKNOWN;
      break;
  }

  //Initialize the Sick LMS 2xx
  try {
    this->sick_lms->Initialize(desired_baud);
    this->isConnected = true;
  } catch (...) {
    error.addError("Initialize_failed", "Initialize failed! Are you using the correct device path?");
    this->isConnected = false;
    delete this->sick_lms;
    this->sick_lms = NULL;
    return false;
  }
  return true;
  // Bouml preserved body end 00026C71
}


# Anemometer Unit  

======================================================

### Objective
Make a reliable cup-and-vane anemometer that can communicate with the central unit via [insert protocol].  
The design uses hall effect sensor tecnology to detect the components movement with it still being waterproof. 

### Repository contents  
- `anemometer_code` contains the arduino sketch that runs on the Arduino MKR 1010 WIFI
- `3mf_files` contains the 3mf files to print the anemometer parts

### Hardware  
- 1 x Arduino MKR 1010 WIFI
- 1 x AS5600 hall effect rotary encoder
- 2 x A3144 hall effect sensor
- 1 x 3.7V [insert capacity] LiPo battery with jst connector

### Current state  
The code has been designed to report a sample of the rotation per minute of the cup every sample period.  
Relative wind angle measurement is not yet implemented.  
The code is NOT TESTED, therefore not guaranteed to run.  
The content of the `3mf_files` directory is incomplete, it's missing the files for the vane of the anemometer.

### TODO
- [ ] Test and validate first code
- [ ] Troubleshoot first code
- [ ] Add the vane .3mf files to `3mf_files`
- [ ] Update the website with the info in this very document
- [ ] Implement relative wind angle readings from the AS5600

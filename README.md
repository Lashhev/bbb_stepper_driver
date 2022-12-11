# BeagleBone Black GPIO Software Driver for controlling the TB6600 Stepper Motor Hardware Driver

This software driver implements the control of a NEMA-type stepper motor by means of a TB6600 hardware driver using the GPIO of a BeagleBone Black single-board computer.
It is possible to control such parameters of the stepper motor as:

- Acceleration
- Speed
- Position

## Hardware description

The figure below shows the GPIO diagram of the BeagleBone Black single-board computer.
![beaglebone_pinout](data/beaglebone-black-pinout.jpg)

## How to connect BeagleBone Black and TB6600

To test the software driver, only a circuit with a common cathode was used, as shown in the figure below. For more information about TBB6600, follow the link [TBB6600](doc/drayver-TB6600-manual.pdf)

![TBB6600_connection](data/TBB6600_connection.png)

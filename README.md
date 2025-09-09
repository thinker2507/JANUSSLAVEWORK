CanSat GNSS Flight Software

Overview
This flight software is designed for a CanSat using a Quectel L89HA GNSS module. The software performs the following functions:

* Reads NMEA \$GNGGA sentences from the GNSS module.
* Parses UTC time, latitude, longitude, and MSL altitude.
* Implements a flight state machine for the states: IDLE, ASCENT, APOGEE, DESCENT, PAYLOAD DEPLOYED, and LANDED.
* Prints all relevant data to the Serial Monitor for real-time monitoring.

Hardware Setup

* Arduino board (Uno, Nano, or MEGA)
* Quectel L89HA GNSS Module

Connections

* GNSS Module TX/RX to Arduino TX0/RX0 (hardware UART)
* Arduino USB connection to PC for Serial Monitor output

GNSS Configuration

* Baud rate: 9600 bps (default)
* NMEA output: only \$GNGGA enabled
* Quectel commands to configure the module:
  Serial.println("\$PQTXT,W,0");       // Disable all NMEA sentences
  Serial.println("\$PQTXT,W,1,GNGGA"); // Enable only GNGGA

Flight State Machine

* IDLE: CanSat is stationary on the ground.
* ASCENT: CanSat is ascending.
* APOGEE: CanSat has reached its peak altitude.
* DESCENT: CanSat is descending without the payload deployed.
* PAYLOAD DEPLOYED: CanSat is descending after the payload has been deployed at 75% of peak altitude.
* LANDED: CanSat has landed on the ground.

Software Logic

* Previous altitude is compared with current altitude to determine ascent or descent.
* Peak altitude is recorded when ascent stops.
* Payload deployment is triggered at 75% of peak altitude.
* All flight data (time, latitude, longitude, altitude, state) is printed to the Serial Monitor.

Serial Monitor Output
The output format for each reading is:
Time: HH\:MM\:SS | Lat: <latitude> | Lon: <longitude> | Alt: <altitude> m | State: \<flight\_state>

Notes

* The software relies on a valid GNSS fix for accurate altitude and position readings.
* Only \$GNGGA sentences are parsed to reduce UART traffic and simplify processing.
* Adjust the payload deployment logic or thresholds according to your mission requirements.




TASK 2

Defines two data trace objects for Plotly plots:

data2D: stores altitude against UTC time seconds (x,y arrays).

data3D: stores latitude, longitude, and altitude arrays for 3D scatter plot.

Initializes two Plotly plots with titles and axis labels on page load.


Splits the incoming NMEA line by commas.

Performs a relaxed check to allow partial sentences (minimum 6 fields for lat/lon).

Parses UTC time, latitude, longitude, and altitude from known standard indices.

Uses conversion functions to convert lat/lon formats and UTC time.

Returns an object with numeric utcSeconds, lat, lon, alt and raw UTC string.

Returns null if critical data missing or invalid.


Parses $GNGGA sentences, extracts coordinates and timing.

Plots live altitude (vs UTC time) and 3D geographic path.


TASK 3

Had to switch operating systems only have a file where components are rearranged no connections .
So could not complete task :=(

String nmeaSentence = "";
double latitude = 0, longitude = 0, altitude = 0;
int hour = 0, minute = 0, second = 0;

// Flight state machine
enum FlightState {
  IDLE,
  ASCENT,
  APOGEE,
  DESCENT,
  PAYLOAD_DEPLOYED,
  LANDED
};

FlightState currentState = IDLE;
double peakAltitude = 0;
bool payloadDeployed = false;

double prevAltitude = 0; // previous altitude to detect ascent/descent

void setup() {
  Serial.begin(9600);  // Hardware UART: TX0/RX0

  Serial.println("Initializing GNSS Module...");

  // Configure L89HA to output ONLY GNGGA
  Serial.println("$PQTXT,W,0");       // Disable all NMEA sentences
  delay(100);
  Serial.println("$PQTXT,W,1,GNGGA"); // Enable GNGGA only
  delay(100);

  Serial.println("GNSS Configured: Only GNGGA enabled");
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') { // End of NMEA sentence
      parseGGA(nmeaSentence);   // Parse GNGGA
      nmeaSentence = "";
      
      // Update flight state using previous-altitude logic
      updateFlightState(altitude);

      // Print flight data
      Serial.print("Time: ");
      print2digits(hour); Serial.print(":");
      print2digits(minute); Serial.print(":");
      print2digits(second);

      Serial.print(" | Lat: "); Serial.print(latitude, 6);
      Serial.print(" | Lon: "); Serial.print(longitude, 6);
      Serial.print(" | Alt: "); Serial.print(altitude, 2);
      Serial.print(" m | State: ");
      printState(currentState);
      Serial.println();

    } else if (c != '\r') {
      nmeaSentence += c; // accumulate sentence
    }
  }
}

// ------------------- PARSE GNGGA -------------------
void parseGGA(String s) {
  // $GNGGA,hhmmss.sss,lat,N,lon,E,fix,numSV,HDOP,alt,M,...
  String parts[15];
  int idx = 0;
  while (s.indexOf(",") != -1) {
    parts[idx++] = s.substring(0, s.indexOf(","));
    s = s.substring(s.indexOf(",")+1);
  }
  parts[idx] = s;

  // Parse time
  if(parts[1].length() >= 6){ 
    hour = parts[1].substring(0,2).toInt();
    minute = parts[1].substring(2,4).toInt();
    second = parts[1].substring(4,6).toInt();
  }

  // Parse latitude & longitude
  if(parts[2].length() > 0 && parts[4].length() > 0){ 
    latitude = convertToDecimal(parts[2].toFloat(), parts[3].charAt(0));
    longitude = convertToDecimal(parts[4].toFloat(), parts[5].charAt(0));
  }

  // Parse altitude (MSL)
  if(parts[9].length() > 0){ 
    altitude = parts[9].toFloat();
  }
}

// Convert NMEA lat/lon to decimal degrees
double convertToDecimal(float val, char dir) {
  int deg = int(val / 100);
  float minutes = val - deg*100;
  double dec = deg + minutes/60.0;
  if(dir == 'S' || dir == 'W') dec = -dec;
  return dec;
}

// ------------------- FLIGHT STATE MACHINE -------------------
void updateFlightState(double alt) {
    double deltaAlt = alt - prevAltitude;
    prevAltitude = alt; // store current for next loop

    switch(currentState){
        case IDLE:
            if(deltaAlt > 0) currentState = ASCENT; // started going up
            break;

        case ASCENT:
            if(deltaAlt <= 0) {  // ascent stopped
                currentState = APOGEE;
                peakAltitude = alt; // record peak
            }
            break;

        case APOGEE:
            currentState = DESCENT;
            break;

        case DESCENT:
            if(!payloadDeployed && alt <= 0.75 * peakAltitude){
                currentState = PAYLOAD_DEPLOYED;
                payloadDeployed = true;
            } else if(alt <= 0){
                currentState = LANDED;
            }
            break;

        case PAYLOAD_DEPLOYED:
            if(alt <= 0) currentState = LANDED;
            break;

        case LANDED:
            // final state
            break;
    }
}

// ------------------- HELPERS -------------------
void printState(FlightState s){
  switch(s){
    case IDLE: Serial.print("IDLE"); break;
    case ASCENT: Serial.print("ASCENT"); break;
    case APOGEE: Serial.print("APOGEE"); break;
    case DESCENT: Serial.print("DESCENT"); break;
    case PAYLOAD_DEPLOYED: Serial.print("PAYLOAD DEPLOYED"); break;
    case LANDED: Serial.print("LANDED"); break;
  }
}

void print2digits(int val){
  if(val < 10) Serial.print("0");
  Serial.print(val);
}

//#include <SPI.h>
//#include <Controllino.h>

//Open USB/Serial port for transmitting data
void setup() {
  Serial.begin(9600);
}

// Definition of constants
unsigned long previous = 0; //timing of measurements
const long interval = 1000; //timing of measurements
float flow_old = 0; //smoothing of data
float dens_old = 0; //smoothing of data
float temp_old = 0; //smoothing of data

//Measuremnt loop
void loop() { 

  unsigned long current = millis(); //time at the beginning of measurements

  // Very low degree (20%) of data smoothing is applied
  float flow_ = 0.8*analogRead(A0) + 0.2*flow_old; //binomial smoothing formula for flow
  float dens_ = 0.8*analogRead(A1) + 0.2*dens_old; //binomial smoothing formula for density
  float temp_ = 0.8*analogRead(A2) + 0.2*temp_old; //binomial smoothing formula for temperature

  // These formulas convert the current output (4-20 mA) from Emerson transmitter 
  // to voltage input for Controllino. THe coefficients have been obtained from calibration tests.
  float flow = flow_*0.03*2.123 - 1.83;
  float dens = dens_*0.03*276.4 - 281.2;
  float temp = temp_*0.03*25.27 - 25.58;
  if(flow < 0) flow = 0;
  if(dens < 0) dens = 0;
  if(temp < 0) temp = 0;

// At every 1000 milliseconds, this if-statement takes all the measurements
// and parses a JSON message of them.
if (current - previous >= interval) {
    previous = current;

      String msg = " {\"flow\": " + String(flow) + ", "
                  + "\"dens\": " + String(dens) + ", "
                  + "\"temp\": " + String(temp) + "}";

      Serial.println(msg); //This prints the JSON message to USB port
      
}

// Update of the smoothing formulas
flow_old = flow_;
dens_old = dens_;
temp_old = temp_;

} // End of measurement loop.

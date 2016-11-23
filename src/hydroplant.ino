void setup(){
  Serial.begin(57600);
}

void loop(){

  int moisture = analogRead(A0);


  Serial.print("Moisture Sensor Value: ");
  Serial.println(moisture);
  delay(500);
}

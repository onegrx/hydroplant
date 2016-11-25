#include <EtherCard.h>
#include <dht11.h>

// Static configuration
#define STATIC 0
#if STATIC
static byte myip[] = {192, 168, 1, 200};
static byte gwip[] = {192, 168, 1, 100};
#endif

dht11 DHT11;
BufferFiller bfill;

static byte mymac[] = {0xD0, 0x69, 0x69, 0x2D, 0x30, 0x31};
byte Ethernet::buffer[1500];

const int dhtPin = 2;
const int moisturePin = A0;
const int lightPin = A1;

word homePage(int t, int h, int m, int l) {
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
  "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "<meta http-equiv='refresh' content='5'/>"
    "<meta charset=\"utf-8\" />"
    "<style>"
    "body { background-color: #333333; text-align: center; color: #33b5d3; font-size: 22px; font-family: 'Gloria Hallelujah', cursive; }"
    "p { display: inline; margin-left: 12px; }"
    "div { margin: 20px; }"
    "</style>"
    "<link href=\"https://maxcdn.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css\" rel=\"stylesheet\">"
    "<link href=\"https://fonts.googleapis.com/css?family=Gloria+Hallelujah\" rel=\"stylesheet\">"
    "<title>HydroPlant</title>"
    "</head>"
    "<body>"
    "<h1> HydroPlant Monitoring </h1>"
    "<div class=\"temperature\"><i class=\"fa fa-thermometer-three-quarters\"></i><p>Temperature: $D&deg;C</p></div>"
    "<div class=\"humidity\"><i class=\"fa fa-tint\"></i><p>Humidity: $D%</p></div>"
    "<div class=\"moisture\"><i class=\"fa fa-pagelines\"></i><p>Moisture: $D</p></div>"
    "<div class=\"light\"><i class=\"fa fa-lightbulb-o\"></i><p>Light: $D</p></div>"
    "</body>"
    "</html>"), t, h, m, l);
  return bfill.position();
}


void setup() {

  Serial.begin(57600);

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) {
    Serial.println("Failed to access Ethernet controller");
  }

  #if STATIC
  ether.staticSetup(myip, gwip);
  #else
  if (!ether.dhcpSetup())
  Serial.println("DHCP failed");
  #endif

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

}

void loop(){

  if (ether.packetLoop(ether.packetReceive())) {
    int t = readTemperature();
    int h = readHumidity();
    int m = readMoisture();
    int l = readLight();
    ether.httpServerReply(homePage(t, h, m, l));
  }

}

int readTemperature() {
  int dhtData = DHT11.read(dhtPin);
  return (int) DHT11.temperature;
}

int readHumidity() {
  int dhtData = DHT11.read(dhtPin);
  return (int) DHT11.humidity;
}

int readMoisture() {
  return (int) analogRead(moisturePin);
}

int readLight() {
  return (int) analogRead(lightPin);
}

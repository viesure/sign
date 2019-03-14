#include <AWS_IOT.h>
#include <WiFi.h>

AWS_IOT hornbill;

char WIFI_SSID[] = "";
char WIFI_PASSWORD[] = "";
char HOST_ADDRESS[] = "yoururl.iot.eu-central-1.amazonaws.com";
char CLIENT_ID[] = "your client id";
char TOPIC_NAME[] = "$aws/things/test/shadow/update";


int status = WL_IDLE_STATUS;
int msgCount = 0, msgReceived = 0;

char payload[512];
char rcvdPayload[512];

uint8_t ledR = 4;
uint8_t ledG = 21;
uint8_t ledB = 5;

void mySubCallBackHandler (char *topicName, int payloadLen, char *payLoad)
{
  strncpy(rcvdPayload, payLoad, payloadLen);
  rcvdPayload[payloadLen] = 0;
  msgReceived = 1;
}

void writeRGB(int red, int green, int blue) {

  ledcWrite(1, red);
  ledcWrite(2, green);
  ledcWrite(3, blue);
}


void setup() {
  Serial.begin(115200);
  delay(2000);

  ledcAttachPin(ledR, 1); 
  ledcAttachPin(ledG, 2);
  ledcAttachPin(ledB, 3);

  ledcSetup(1, 12000, 8); // 12 kHz PWM, 8-bit resolution
  ledcSetup(2, 12000, 8);
  ledcSetup(3, 12000, 8);

  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(WIFI_SSID);
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // wait 5 seconds for connection:
    delay(5000);
  }

  Serial.println("Connected to wifi");

  if (hornbill.connect(HOST_ADDRESS, CLIENT_ID) == 0)
  {
    Serial.println("Connected to AWS");
    delay(1000);

    if (0 == hornbill.subscribe(TOPIC_NAME, mySubCallBackHandler))
    {
      Serial.println("Subscribe Successfull");
    }
    else
    {
      Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
      while (1);
    }
  }
  else
  {
    Serial.println("AWS connection failed, Check the HOST Address");
    while (1);
  }

  delay(2000);

}

void loop() {

  if (msgReceived == 1)
  {
    msgReceived = 0;
    Serial.print("Received Message:");
    Serial.println(rcvdPayload);

    String str(rcvdPayload);

    int firstIndex = str.indexOf(',');
    int lastIndex = str.lastIndexOf(',');

    Serial.println(firstIndex);
    Serial.println(lastIndex);

    int red = str.substring(0, firstIndex).toInt();
    int green = str.substring(firstIndex + 1, lastIndex).toInt();
    int blue = str.substring(lastIndex + 1, str.length()).toInt();

    Serial.println(red);
    Serial.println(green);
    Serial.println(blue);

    writeRGB(red, green, blue);

  }

  vTaskDelay(1000 / portTICK_RATE_MS);

}

#include <PubSubClient.h>//for MQTT
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

#define LED_RED_PIN            27
#define LED_GREEN_PIN          26
#define LED1 27
#define LED2 26
#define LED3 25
#define LED4 33
#define LED5 32
#define LED6 14
#define LED7 12
#define LED8 13
#define LED9 15

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

volatile unsigned char blinkFlagArr[9]={0};
volatile unsigned int numberOfBlinksArr[9]={0};

const char* mqtt_server = "mqtt.thingspeak.com";
char mqttUserName[] = "your name";  // Can be any name.
char mqttPass[] = "your mqtt api key";  // Change this your MQTT API Key from Account > MyProfile.

//create wifi client class' object, name it espClient
WiFiClient espClient;
//create mqtt client object from PubSubClient Class
PubSubClient client(espClient);



void setup_wifi()
{ 
//connect to wifi with ssid/password 
    wifiMulti.addAP("ssid1", "password1");
    wifiMulti.addAP("ssid1", "password1");
    wifiMulti.addAP("ssid3", "password3");
	Serial.println("Connecting Wifi...");
	while(wifiMulti.run() != WL_CONNECTED);
	{
		delay(1000);
		Serial.print(".");
	}
    
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
}


//reconnect to mqtt server
void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP32";
        clientId += String(random(0xffff), HEX);//added random number in Client ID because each id should be unique
        // Attempt to connect
        if (client.connect(clientId.c_str(),mqttUserName,mqttPass))
        {
            Serial.println("connected");
            //replace 361111 with your channel id
            client.subscribe("channels/361111/subscribe/fields/field1");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            vTaskDelay(5000);
        }
    }
}

//callback is called automatically when MQTT message is received
void callback(char* topic, byte* payload, unsigned int length)
{
    String rxMessage="";
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
   
    for (int i = 0; i < length; i++)
    {
        rxMessage+=(char)payload[i];
    }
    Serial.println(rxMessage);
    if(rxMessage.equals("green on"))
    {
    	numberOfBlinksArr[1]=0;//stop blinking if already blinking
    	digitalWrite(LED_GREEN_PIN, HIGH);
    }
    else if(rxMessage.equals("green off"))
    {
    	numberOfBlinksArr[1]=0;//stop blinking if already blinking
    	digitalWrite(LED_GREEN_PIN, LOW);
    }
    else if(rxMessage.equals("red on"))
    {
    		numberOfBlinksArr[0]=0;//stop blinking if already blinking
    		digitalWrite(LED_RED_PIN, HIGH);    		
    }
    else if(rxMessage.equals("red off"))
    {
    		numberOfBlinksArr[0]=0;//stop blinking if already blinking
    		digitalWrite(LED_RED_PIN, LOW);    		
    }
    else if(rxMessage.indexOf("blinkred")>=0)
    {
    	String numString;
    	//blinkFlag=1;
    	blinkFlagArr[0]=1;	    			    	
	    numString=rxMessage.substring(9+rxMessage.indexOf("blinkred"));
    	numberOfBlinksArr[0]=numString.toInt();  
    	//ledToBlink=LED_RED_PIN;  	
    }
    else if(rxMessage.indexOf("blinkgreen")>=0)
    {
    	String numString;
    	blinkFlagArr[1]=1;
    	numString=rxMessage.substring(11+rxMessage.indexOf("blinkgreen"));
    	//Serial.print(numString);
    	numberOfBlinksArr[1]=numString.toInt();  
    	//ledToBlink=LED_GREEN_PIN;  	
    }
    else if(rxMessage.indexOf("blink")>=0)
    {
    	String numString;
    	static unsigned char LedToBlinkNum=2;
    	unsigned int num;    	
    	numString=rxMessage.substring(5);
    	num=numString.toInt();
    	if(num>0)
    	{    		
			blinkFlagArr[LedToBlinkNum]=1;
	    	//Serial.println(numString);	    		    	
	    	numberOfBlinksArr[LedToBlinkNum]=num;
	    	LedToBlinkNum++;
	    	if(LedToBlinkNum>=9)
	    		LedToBlinkNum=2;
    	}
    }
    else if(rxMessage.equals("allreset"))
    {
    	for(char counter=0;counter<=8;counter++)
    		{
    			numberOfBlinksArr[counter]=0;    			
    		}
    		digitalWrite(LED_RED_PIN, LOW);
    		digitalWrite(LED_GREEN_PIN, LOW);
    }
}

void blinkTask1(void *pvParameters)
{
	while(1)
	{
		vTaskDelay(100);
		if(blinkFlagArr[0]==1)
		{
			blinkFlagArr[0]=0;
			for(unsigned int loopCount=0;loopCount<numberOfBlinksArr[0];loopCount++)
			{
				digitalWrite(LED1, HIGH);
				delay(150);
				digitalWrite(LED1, LOW);
				delay(400);
			}
			numberOfBlinksArr[0]=0;
		}
	}
}
void blinkTask2(void *pvParameters)
{
	while(1)
	{
		vTaskDelay(120);
		if(blinkFlagArr[1]==1)
		{
			blinkFlagArr[1]=0;
			for(unsigned int loopCount=0;loopCount<numberOfBlinksArr[1];loopCount++)
			{
				digitalWrite(LED2, HIGH);
				delay(200);
				digitalWrite(LED2, LOW);
				delay(500);
			}
			numberOfBlinksArr[1]=0;
		}
	}
}
void blinkTask3(void *pvParameters)
{
	while(1)
	{
		vTaskDelay(140);
		if(blinkFlagArr[2]==1)
		{
			blinkFlagArr[2]=0;
			for(unsigned int loopCount=0;loopCount<numberOfBlinksArr[2];loopCount++)
			{
				digitalWrite(LED3, HIGH);
				delay(250);
				digitalWrite(LED3, LOW);
				delay(600);
			}
			numberOfBlinksArr[2]=0;
		}
	}
}
void blinkTask4(void *pvParameters)
{
	while(1)
	{
		vTaskDelay(200);
		if(blinkFlagArr[3]==1)
		{
			blinkFlagArr[3]=0;
			for(unsigned int loopCount=0;loopCount<numberOfBlinksArr[3];loopCount++)
			{
				digitalWrite(LED4, HIGH);
				delay(300);
				digitalWrite(LED4, LOW);
				delay(700);
			}
			numberOfBlinksArr[3]=0;
		}
	}
}
void blinkTask5(void *pvParameters)
{
	while(1)
	{
		vTaskDelay(250);
		if(blinkFlagArr[4]==1)
		{
			blinkFlagArr[4]=0;
			for(unsigned int loopCount=0;loopCount<numberOfBlinksArr[4];loopCount++)
			{
				digitalWrite(LED5, HIGH);
				delay(350);
				digitalWrite(LED5, LOW);
				delay(800);
			}
			numberOfBlinksArr[4]=0;
		}
	}
}
void blinkTask6(void *pvParameters)
{
	while(1)
	{
		vTaskDelay(275);
		if(blinkFlagArr[5]==1)
		{
			blinkFlagArr[5]=0;
			for(unsigned int loopCount=0;loopCount<numberOfBlinksArr[5];loopCount++)
			{
				digitalWrite(LED6, HIGH);
				delay(400);
				digitalWrite(LED6, LOW);
				delay(900);
			}
			numberOfBlinksArr[5]=0;
		}
	}
}
void blinkTask7(void *pvParameters)
{
	while(1)
	{
		vTaskDelay(315);
		if(blinkFlagArr[6]==1)
		{
			blinkFlagArr[6]=0;
			for(unsigned int loopCount=0;loopCount<numberOfBlinksArr[6];loopCount++)
			{
				digitalWrite(LED7, HIGH);
				delay(450);
				digitalWrite(LED7, LOW);
				delay(1000);
			}
			numberOfBlinksArr[6]=0;
		}
	}
}
void blinkTask8(void *pvParameters)
{
	while(1)
	{
		vTaskDelay(400);
		if(blinkFlagArr[7]==1)
		{
			blinkFlagArr[7]=0;
			for(unsigned int loopCount=0;loopCount<numberOfBlinksArr[7];loopCount++)
			{
				digitalWrite(LED8, HIGH);
				delay(500);
				digitalWrite(LED8, LOW);
				delay(1050);
			}
			numberOfBlinksArr[7]=0;
		}
	}
}
void blinkTask9(void *pvParameters)
{
	while(1)
	{
		vTaskDelay(450);
		if(blinkFlagArr[8]==1)
		{
			blinkFlagArr[8]=0;
			for(unsigned int loopCount=0;loopCount<numberOfBlinksArr[8];loopCount++)
			{
				digitalWrite(LED9, HIGH);
				delay(550);
				digitalWrite(LED9, LOW);
				delay(1200);
			}
			numberOfBlinksArr[8]=0;
		}
	}
}


void setup()
{
	 Serial.begin(115200);
	//pinMode(LED_RED_PIN, OUTPUT);           // set pin to input	
	//pinMode(LED_GREEN_PIN, OUTPUT);           // set pin to input	
	pinMode(LED1, OUTPUT);           // set pin to input	
	pinMode(LED2, OUTPUT);           // set pin to input	
	pinMode(LED3, OUTPUT);           // set pin to input	
	pinMode(LED4, OUTPUT);           // set pin to input	
	pinMode(LED5, OUTPUT);           // set pin to input	
	pinMode(LED6, OUTPUT);           // set pin to input	
	pinMode(LED7, OUTPUT);           // set pin to input	
	pinMode(LED8, OUTPUT);           // set pin to input	
	pinMode(LED9, OUTPUT);           // set pin to input
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);   
    xTaskCreatePinnedToCore(blinkTask1, "loop5", 4096, (void *)3, 2, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(blinkTask2, "loop6", 4096, (void *)4, 2, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(blinkTask3, "loop7", 4096, (void *)5, 2, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(blinkTask4, "loop8", 4096, (void *)6, 2, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(blinkTask5, "loop9", 4096, (void *)7, 2, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(blinkTask6, "loop10", 4096, (void *)8, 2, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(blinkTask7, "loop11", 4096, (void *)9, 2, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(blinkTask8, "loop12", 4096, (void *)10, 2, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(blinkTask9, "loop12", 4096, (void *)11, 2, NULL, ARDUINO_RUNNING_CORE);
}

void loop()
{
	if (!client.connected())
    {
        reconnect();
    }
    client.loop();
}


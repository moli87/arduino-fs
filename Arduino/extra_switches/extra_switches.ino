#include <PinChangeInt.h> // necessary otherwise we get undefined reference errors.
#include <AdaEncoder.h>

int8_t clicks=0;
char rotary_id=0;
encoder *thisEncoder;

void setup()
{
  Serial.begin(9600);
  Serial.flush();
  Serial.print("boot\n");
  AdaEncoder::addEncoder('p', 2, 3);
  AdaEncoder::addEncoder('1', 5, 4); 
  AdaEncoder::addEncoder('h', 6, 7); 
  AdaEncoder::addEncoder('o', 9, 8);   
  AdaEncoder::addEncoder('a', 10, 11);  
  
}

void loop()
{
  Serial.print("run\n");  
  delay (500);
  thisEncoder= NULL;
  thisEncoder=AdaEncoder::genie(&clicks, &rotary_id);
  if (thisEncoder != NULL) {
    //Serial.print  (clicks);
    if (clicks >5 ||  clicks <-5)
        clicks*= 5;
    send_rotary (-clicks, rotary_id);
    thisEncoder->clicks= 0;
  }
}

void send_rotary(int val, char name)
{
  char buffer[20];
  uint16_t num; 
  if (name == '1')
  {
      sprintf(buffer, "bank1:%d\n" , val); 
  }

  else if (name == 'h')
  {
    sprintf(buffer, "heading:%d\n" , val); 
  }

  else if (name == 'o')
  {
   
      sprintf(buffer, "obs:%d\n" , val); 
    
  }
  else if (name == 'p')
  {
   
      sprintf(buffer, "ap:%d\n" , val ); 
    
  }

  else if (name == 'a')
  {
      sprintf(buffer, "adf:%d\n",  val); 
  }
  Serial.write(buffer); 
}

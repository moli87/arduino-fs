

#define STRLEN 20

char buffer[STRLEN];
int  bufferIndex = 0;
AdvButton park=AdvButton(4, "parkingb", NULL, single_button_on, single_button_off, btn_Digital);

void setup()
{
  Serial.begin(115200);
  Serial.flush();
  pinMode (13, OUTPUT);
  
}

void loop()
{
  if( Serial.available())
  {
    char ch = Serial.read();
    if( ch == '\n')  // is this the terminating carriage return
    {
      buffer[ bufferIndex ] = 0; // terminate the strin
      bufferIndex = 0;  // reset the index ready for another string
      parse_string(buffer);
      // do something with the string
    }
    else
      buffer[ bufferIndex++ ] = ch; // add the character into the buffer
  }
  ButtonManager::instance()->checkButtons();  
  check_input();
} 

void send_init()
{
  Serial.write ("init\0");
}

void parse_string (char* buffer)
{
  char *name = NULL, *value = NULL;
  //Serial.write(buffer);
  name =strtok (buffer, ":");
  value =strtok( NULL, ":");
  if (name && value) {
    handle_command (name, value);
  }
}

void handle_command (char* name,char* value)
{
  if(strcmp(name, "resend"))
  {
    send_init();
    ButtonManager::instance()->triggerAll();
  }
}

void send_value (char* name,int value)
{
  char buffer [20];
  sprintf (buffer, "%s:%02x\0", name, value);
  Serial.write (buffer);
}

void send_string (char* name,char* value)
{
  char buffer [20];
  sprintf (buffer, "%s:%s\0", name, value);
  Serial.write (buffer);
}

void check_input ()
{
}

void single_button_on(AdvButton *but)
{
  send_value(but->getName(), 1);
}

void single_button_off(AdvButton *but)
{
  send_value(but->getName(), 0);
}

void lights_on(AdvButton *but)
{
  but->setBit();
  send_value (but->getName(), but->getBytes());
}

void lights_off(AdvButton *but)
{
  but->clearBit();
  send_value (but->getName(), but->getBytes());
}




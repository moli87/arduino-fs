#include <SPI.h>
#include <AdvButton.h>
#include <ButtonManager.h>

#define ANALOG_DELAY 250
#define NAV1 53 //ss
#define STRLEN 30

char buffer[STRLEN];
int  bufferIndex = 0;
char buffer3[STRLEN];
int  bufferIndex3 = 0;
#define NAV1 1
#define NAV2 0
#define COM1 3
#define COM2 2

#define ANALOG_SENSITIVITY 5

#define ADF1 0
#define ADF2 1
#define TRANSPONDER 2
#define DME1 3
#define DME1TIME 4
#define DME2 5
#define DME2TIME 6

#define OBS1 0
#define OBS2 1
#define ALTITUDE 0
#define VERTICAL 1

#define BANK1CONTROL 0
#define BANK2CONTROL 1
#define ADFCONTROL 2
#define OBSCONTROL 3
#define APCONTROL 4
#define MAGCONTROL 8


#define BANK1MASTER 53
#define BANK1SLAVE 49
#define BANK2MASTER 48
#define BANK2SLAVE 47
#define ADF 46
#define OBS 45
#define ALTITUDE_PIN 44
#define HEADING_PIN 43

#define FLAPS_PIN A5
#define SPOILER_PIN A6
#define AILERON_PIN A7
#define RUDDER_PIN A3

#define SWAP_BANK1_PIN 12
#define SWAP_BANK2_PIN 2

#define NAVIGATION_PIN 7
#define BEACON_PIN 8
#define LANDING_PIN 16
#define TAXI_PIN 17
#define STROBE_PIN 18
#define INSTRUMENTS_PIN 19
#define RECOGNITION_PIN 20
#define WING_PIN 21
#define LOGO_PIN 22
#define CABIN_PIN 23
#define BATTERY_PIN 24
#define AVIONICS_PIN 25
#define ALTERNATE_PIN 26
#define FUEL_PIN 27
#define DEICE_PIN 28
#define PITOT_PIN 29
#define NAV_GPS_PIN 30
#define KEY_PIN 21
#define MASTER_PIN 32
#define THROTTLE_PIN 33
#define HEADING_HOLD_PIN 34
#define ALTITUDE_HOLD_PIN 35
#define NAV_HOLD_PIN 36
#define VERTICAL_SPEED_HOLD_PIN 37
#define ATTITUDE_PIN 38
#define APPROACH_MODE_PIN 39
#define SPEED_HOLD_PIN 40
#define GEAR_PIN 41

#define STEP_BANK1_PIN 42

#define PARKING_PIN 9
int last_flaps = 0; 
int last_spoiler = 0;   
int last_aileron = 0;
int last_rudder = 0;
unsigned long last_flaps_time = 0;
unsigned long last_spoiler_time = 0;
unsigned long last_aileron_time = 0;
unsigned long last_rudder_time = 0; 
int step_bank1= 1;
int step_bank2=1;
int transponder_step = 10000; 

char* radio_master[4] = {
  "1111",  "2222", "3333", "4444"};
char* radio_slave[4] = {
  "5555",  "6666", "7777", "8888"};
char* adf[7] = {
  "0111",  "0222", "7333", "0444", "0555","0666", "0777"};
char* obs[2] = {
  "v333",  "v444"};
char* autopilot_altitude[2] = {
  "1111",  "2222"};
char* heading = "v000";
int active_bank_one=COM1;
int active_bank_two=NAV1;
int active_ADF = ADF1;
int active_obs = OBS1;
int active_altitude = ALTITUDE;

int8_t clicks=0;
char rotary_id=0;


AdvButton park=AdvButton(PARKING_PIN, "parkingb", NULL, single_button_on, single_button_off, btn_Digital);
AdvButton swap_bank_one_button=AdvButton(SWAP_BANK1_PIN, "swapbank1", swap_bank1, btn_Digital);
AdvButton swap_bank_to_button=AdvButton(SWAP_BANK2_PIN, "swapbank2", swap_bank2, btn_Digital);
AdvButton step_bank_one_button=AdvButton(STEP_BANK1_PIN, "stepbank1", change_step_bank_one, btn_Digital);

AdvButton bank1com1=AdvButton(BANK1CONTROL, "mas3", NULL, select_bank1, NULL, btn_Analog, 0, 200);
AdvButton bank1com2=AdvButton(BANK1CONTROL, "mas2", NULL, select_bank1, NULL, btn_Analog, 200, 400);
AdvButton bank1nab1=AdvButton(BANK1CONTROL, "mas1", NULL, select_bank1, NULL, btn_Analog, 400, 600);
AdvButton bank1nab2=AdvButton(BANK1CONTROL, "mas0", NULL, select_bank1, NULL, btn_Analog, 600, 800);

AdvButton bank_to_come1=AdvButton(BANK2CONTROL, "mas3", NULL, select_bank2, NULL, btn_Analog, 0, 200);
AdvButton bank_to_commit_to=AdvButton(BANK2CONTROL, "mas2", NULL, select_bank2, NULL, btn_Analog, 200, 400);
AdvButton bank2nav1=AdvButton(BANK2CONTROL, "mas1", NULL, select_bank2, NULL, btn_Analog, 400, 600);
AdvButton bank2nav2=AdvButton(BANK2CONTROL, "mas0", NULL, select_bank2, NULL, btn_Analog, 600, 800);

AdvButton magoff=AdvButton(MAGCONTROL, "mas0", NULL, send_magneto, NULL, btn_Analog, 0, 100);
AdvButton magone=AdvButton(MAGCONTROL, "mas1", NULL, send_magneto, NULL, btn_Analog, 100, 200);
AdvButton magtwo=AdvButton(MAGCONTROL, "mas2", NULL, send_magneto, NULL, btn_Analog, 200, 350);
AdvButton magboth=AdvButton(MAGCONTROL, "mas3", NULL, send_magneto, NULL, btn_Analog, 350, 500);
AdvButton magstart=AdvButton(MAGCONTROL, "mas4", NULL, send_magneto, NULL, btn_Analog, 500, 1024);

AdvButton adfadf1=AdvButton(ADFCONTROL, "adf1", NULL, select_adf, NULL, btn_Analog, 0, 140);
AdvButton adfadf2=AdvButton(ADFCONTROL, "adf0", NULL, select_adf, NULL, btn_Analog, 140, 280);
AdvButton adfadf3=AdvButton(ADFCONTROL, "adf2", NULL, select_adf, NULL, btn_Analog, 280, 420);
AdvButton adfadf4=AdvButton(ADFCONTROL, "adf3", NULL, select_adf, NULL, btn_Analog, 420, 560);
AdvButton adfadf5=AdvButton(ADFCONTROL, "adf4", NULL, select_adf, NULL, btn_Analog, 560, 700);
AdvButton adfadf6=AdvButton(ADFCONTROL, "adf5", NULL, select_adf, NULL, btn_Analog, 700, 840);
AdvButton adfadf7=AdvButton(ADFCONTROL, "adf6", NULL, select_adf, NULL, btn_Analog, 840, 980);

//AdvButton obs1=AdvButton(OBSCONTROL, "obs0", NULL, select_obs, NULL, btn_Analog, 0, 400);
//AdvButton obs2=AdvButton(OBSCONTROL, "obs1", NULL, select_obs, NULL, btn_Analog, 400, 800);

AdvButton alt=AdvButton(APCONTROL, "alt0", NULL, select_altitude, NULL, btn_Analog, 0, 400);
AdvButton vspd=AdvButton(APCONTROL, "vsp1", NULL, select_altitude, NULL, btn_Analog, 600, 1024);

//lights
int lights = 0;
AdvButton navigation_lights = AdvButton(NAVIGATION_PIN, "navigation" , NULL, lights_on, lights_off, btn_Digital, 0, 0, &lights, 0);
AdvButton beacon_lights = AdvButton(BEACON_PIN, "beacon" , NULL, lights_on, lights_off, btn_Digital, 0, 0, &lights, 1);
AdvButton landing_lights = AdvButton(LANDING_PIN, "landing" , NULL, lights_on, lights_off, btn_Digital, 0, 0, &lights, 2); 
AdvButton taxi_lights = AdvButton(TAXI_PIN, "taxi" , NULL, lights_on, lights_off, btn_Digital, 0, 0, &lights, 3);
AdvButton strobe_lights = AdvButton(STROBE_PIN, "strobe" , NULL, lights_on, lights_off, btn_Digital, 0, 0, &lights, 4);
AdvButton instrument_lights = AdvButton(INSTRUMENTS_PIN, "instrument" , NULL, lights_on, lights_off, btn_Digital, 0, 0, &lights, 5);
AdvButton recognition_lights = AdvButton(RECOGNITION_PIN, "recognition" , NULL, lights_on, lights_off, btn_Digital, 0, 0, &lights, 6);
AdvButton wing_lights=AdvButton(WING_PIN,  "wing" ,NULL, lights_on, lights_off, btn_Digital, 0, 0, &lights, 7);
AdvButton logo_lights = AdvButton(LOGO_PIN, "logo" , NULL, lights_on, lights_off, btn_Digital, 0, 0, &lights, 8);
AdvButton cabin_lights = AdvButton(CABIN_PIN, "cabin" , NULL, lights_on, lights_off, btn_Digital, 0, 0, &lights, 9);


//systems
AdvButton battery_but_in = AdvButton(BATTERY_PIN, "battery" , NULL, single_button_on, single_button_off, btn_Digital);
AdvButton avionics_button = AdvButton(AVIONICS_PIN, "avionics" , NULL, single_button_on, single_button_off, btn_Digital);
AdvButton alternator_but_will=AdvButton(ALTERNATE_PIN, "alternator" , NULL, single_button_on, single_button_off, btn_Digital);
AdvButton fuel_button = AdvButton(FUEL_PIN, "fuel" , NULL, single_button_on, single_button_off, btn_Digital);
AdvButton deice_but_in = AdvButton(DEICE_PIN, "deice" , NULL, single_button_on, single_button_off, btn_Digital);
AdvButton peter_heat = AdvButton(PITOT_PIN, "pitot" , NULL, single_button_on, single_button_off, btn_Digital);
AdvButton nav_gps = AdvButton(NAV_GPS_PIN, "nav_gps" , NULL, single_button_on, single_button_off, btn_Digital); 
AdvButton gear = AdvButton(GEAR_PIN, "gear" , NULL, single_button_on, single_button_off, btn_Digital); 

AdvButton key_lock = AdvButton(KEY_PIN, "init" , NULL, single_button_on, single_button_off, btn_Digital); 

//autopilot
AdvButton master_autopilot = AdvButton(MASTER_PIN, "master" , NULL, single_button_on, single_button_off, btn_Digital);
AdvButton throttle = AdvButton(THROTTLE_PIN, "throttle" , NULL, single_button_on, single_button_off, btn_Digital); 
AdvButton heading_hold = AdvButton(HEADING_HOLD_PIN, "hhold" , toggle_button, btn_Digital); 
AdvButton altitude_hold = AdvButton(ALTITUDE_HOLD_PIN, "ahold" , toggle_button, btn_Digital); 
AdvButton nav_hold = AdvButton(NAV_HOLD_PIN, "navhold" , toggle_button, btn_Digital); 
AdvButton vs_hold = AdvButton(VERTICAL_SPEED_HOLD_PIN, "vshold" , toggle_button, btn_Digital); 
AdvButton attitude = AdvButton(ATTITUDE_PIN, "winglevel" , toggle_button, btn_Digital); 

AdvButton approach = AdvButton(APPROACH_MODE_PIN, "approachhold" , toggle_button, btn_Digital); 
AdvButton speedds = AdvButton(SPEED_HOLD_PIN, "speedhold" , toggle_button, btn_Digital); 

//AdvButton gear = AdvButton(GEAR_PIN, "gear" , NULL, single_button_on, single_button_off, btn_Digital); 
void setup()
{
  Serial.begin(115200);
  Serial.flush();
  Serial3.begin(9600);
  Serial3.flush();
  pinMode (BANK1MASTER,OUTPUT);
  pinMode (BANK1SLAVE,OUTPUT);
  pinMode (BANK2MASTER,OUTPUT);
  pinMode (BANK2SLAVE,OUTPUT);

  pinMode (ADF,OUTPUT);
  pinMode (OBS,OUTPUT);
  pinMode (HEADING_PIN,OUTPUT);
  pinMode (ALTITUDE_PIN,OUTPUT);
  pinMode(FLAPS_PIN, INPUT);
  pinMode(SPOILER_PIN, INPUT);
  pinMode(AILERON_PIN, INPUT); 
  pinMode(RUDDER_PIN, INPUT);  
  SPI.begin(); 
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  reset7seg(BANK1MASTER);
  reset7seg(BANK1SLAVE);
  reset7seg(BANK2MASTER);
  reset7seg(BANK2SLAVE);  
  reset7seg(ADF);
  reset7seg(OBS);
  reset7seg(HEADING_PIN);
  reset7seg(ALTITUDE_PIN);

  write_led_decimals(BANK1MASTER,0x2);
  write_led_decimals(BANK1SLAVE,0x2);
  write_led_decimals(BANK2MASTER,0x2);
  write_led_decimals(BANK2SLAVE,0x2);
  write_led_decimals(ADF,0x0);
  //write_led_numbers(NAV1, 0x78,0x78,0x78,0x78); //Blank display
  //write_led_numbers(ADF,1,2,3,4);
}

void loop()
{
  //  delay (10000);
  //Serial.print (digitalRead (GEAR_PIN));
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
  if( Serial3.available())
  {
    char ch = Serial3.read();
    if( ch == '\n')  // is this the terminating carriage return
    {
      buffer3[ bufferIndex3 ] = 0; // terminate the strin
      bufferIndex3 = 0;  // reset the index ready for another string
      //Serial.print(buffer3);
      send_rotary(buffer3);

      // do something with the string
    }
    else
      buffer3[ bufferIndex3++ ] = ch; // add the character into the buffer
  }
  ButtonManager::instance()->checkButtons();  
  check_input();

  send_analog("flaps" , FLAPS_PIN, &last_flaps, &last_flaps_time);
  send_analog_trim("ailtrim" , AILERON_PIN, &last_aileron, &last_aileron_time);
  send_analog_trim("ruddertrim" , RUDDER_PIN, &last_rudder, &last_rudder_time);
  send_analog("spoilers" , SPOILER_PIN, &last_spoiler, &last_spoiler_time); 
} 

void send_analog(char* name, int pin, int* last, unsigned long *last_change)
{
  int current = analogRead(pin);
  if (current <100) current = 0;

  if (current >950)  current = 1024;  
  if (current - ANALOG_SENSITIVITY > *last || current + ANALOG_SENSITIVITY < *last)
  {


    if (millis() - *last_change > ANALOG_DELAY){
      *last_change =  millis(); 

      *last = current; 
      send_value(name, map(current, 0, 1024, -16383, 16383));

    }

  }
}

void send_analog_trim(char* name, int pin, int* last, unsigned long *last_change)
{
  int current = analogRead(pin);
  if (current <50) current = 0;

  if (current >950)  current = 1024;
  if (current - ANALOG_SENSITIVITY > *last || current + ANALOG_SENSITIVITY < *last)
  {



    if (millis() - *last_change > ANALOG_DELAY)
    {
      *last = current; 
      *last_change =  millis(); 
      send_value(name, map(current, 0, 1024, -16383, 16383));

    }


  }
}

void send_spoiler(char* name, int pin, int* last, unsigned long *last_change)
{
  int current = analogRead(pin), spoilers;
  if (current - ANALOG_SENSITIVITY > *last || current + ANALOG_SENSITIVITY < *last)
  {
    spoilers = map(current, 0, 1024, 0, 16383);
    if (spoilers > 4000 && spoilers < 5620)
    {
      spoilers = 4800; 
    }
    if (spoilers <= 4000)
    {
      spoilers = 0; 
    }
    if (millis() - *last_change > ANALOG_DELAY){
      *last_change =  millis(); 
      send_value(name, spoilers);
      *last = current; 
    }

  }
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

void update_display (int num)
{
  if (num == 0 || num == 1)
  {
    write_string (BANK1SLAVE, radio_slave [active_bank_one]);
    write_string (BANK1MASTER, radio_master [active_bank_one]);
    write_string (BANK2MASTER, radio_master [active_bank_two]);
    write_string (BANK2SLAVE, radio_slave [active_bank_two]);
  }

  else if (num == 2)
  {
    write_string (ADF, adf [active_ADF]);
  }

  else if (num == 3)
  {
    write_string (OBS, obs [active_obs]);

  }

  else if (num == 4)
  {
    write_string (ALTITUDE_PIN, autopilot_altitude [active_altitude]);

  }

  else if (num == 5)
  {
    write_string (HEADING_PIN, heading);

  }



}


void my_copy (char* target,char* source,int len)
{
  for(int i=0; i<len;i++){
    target [i] =source [i];
  }
}

void handle_command (char* name,char* value)
{
  if(strcmp(name, "resend")== 0)
  {
    ButtonManager::instance()->triggerAll();
  }
  if (strcmp( name, "nav1m")== 0)  {
    my_copy(radio_master[NAV1], value,4);
    update_display (1);
    //write_string (NAV1, value);
  }
  if (strcmp( name, "nav2m")== 0)
  {
    my_copy(radio_master[NAV2], value,4);
    update_display (1);
  }
  if (strcmp( name, "com1m")== 0)
  {
    my_copy(radio_master[COM1], value,4);
    update_display (0);
  }
  if (strcmp( name, "com2m")== 0)
  {
    my_copy(radio_master[COM2], value,4);
    update_display (0);
  }
  if (strcmp( name, "nav1s")== 0)
  {
    my_copy(radio_slave[NAV1], value,4);
    update_display (1);
    //write_string (NAV1,  value, 4);
  }
  if (strcmp( name, "nav2s")== 0)
  {
    my_copy(radio_slave[NAV2],  value, 4);
    update_display (1);
  }
  if (strcmp( name, "com1s")== 0)
  {
    my_copy(radio_slave[COM1],  value, 4);
    update_display (0);
  }
  if (strcmp( name, "com2s")== 0)
  {
    my_copy(radio_slave[COM2],  value, 4);
    update_display (0);
  }
  if (strcmp( name, "adf1")== 0)
  {
    my_copy(adf[ADF1],  value, 4);
    update_display (2);
  }
  if (strcmp( name, "adf2")== 0)
  {
    my_copy(adf[ADF2],  value, 4);
    update_display (2);
  }
  if (strcmp( name, "trans")== 0)
  {
    my_copy(adf[TRANSPONDER],  value, 4);
    update_display (2);
  }
  if (strcmp( name, "dme1")== 0)
  {
    my_copy(adf[DME1],  value, 4);
    update_display (2);
  }
  if (strcmp( name, "dme2")== 0)
  {
    my_copy(adf[DME2],  value, 4);
    update_display (2);
  }
  if (strcmp( name, "dme1time")== 0)
  {
    my_copy(adf[DME1TIME],  value, 4);
    update_display (2);
  }
  if (strcmp( name, "dme2time")== 0)
  {
    my_copy(adf[DME2TIME],  value, 4);
    update_display (2);
  }
  if (strcmp( name, "obs1")== 0)
  {
    my_copy(obs[OBS1],  value, 4);
    update_display (3);
  }
  if (strcmp( name, "obs2")== 0)
  {
    my_copy(obs[OBS2],  value, 4);
    update_display (3);
  }
  if (strcmp( name, "altitude")== 0)
  {
    my_copy(autopilot_altitude[ALTITUDE],  value, 4);
    update_display (4);
  }
  if (strcmp( name, "vertical")== 0)
  {
    my_copy(autopilot_altitude[VERTICAL],  value, 4);
    update_display (4);
  }
  if (strcmp( name, "heading")== 0)
  {
    my_copy(heading,  value, 4);
    update_display (5);
  }
}

void send_value ( char* name,int value)
{
  char buffer [20];
  sprintf (buffer, "%s:%d\n", name, value);
  Serial.write (buffer);
}

void send_string (char* name,char* value)
{
  char buffer [20];
  sprintf (buffer, "%s:%s\n", name, value);
  Serial.write (buffer);
}
void send_rotary(char* input)
{
  char *name = NULL, *value = NULL;
  //Serial.write(buffer);
  name =strtok (input, ":");
  value =strtok( NULL, ":");
  int val=atoi (value);
  if (name && value) {
    char buffer[20];
    uint16_t num; 
    if (strcmp(name,"bank1")==0)
    {
      float  step = 2.5; 
      int last_ditch_it= atoi(&radio_slave[active_bank_one][3]);
      float increasing  = 2.0;
      if (last_ditch_it == 2 || last_ditch_it == 7)
      {
        increasing = 3.0; 
      }
      num = atoi(radio_slave[active_bank_one]);
      //navigation_radios uses50kilohertz_spacing
      if (active_bank_one  == NAV1 || active_bank_one  == NAV2)
      {
        increasing = 5.0;
        step = 5.0; 
      }
      if (step_bank1 == 1)
      {
        increasing = increasing + ((val - 1) * step); 
        num += (int)increasing;
      }

      else
      {
        num += val * step_bank1; 
      }

      if (active_bank_one == NAV1)
      {
        sprintf(buffer, "nav1s:%04d\n" , num); 
      }

      else if (active_bank_one == NAV2)
      {
        sprintf(buffer, "nav2s:%04d\n" , num); 
      }
      else if (active_bank_one == COM1)
      {
        sprintf(buffer, "com1s:%04d\n" , num); 
      }
      else if (active_bank_one == COM2)
      {
        sprintf(buffer, "com2s:%04d\n" , num); 
      }
    }
    else if (strcmp(name, "bank2")==0)
    {
      num = atoi(radio_slave[active_bank_two]);
      num += val  *step_bank2; 

      if (active_bank_two == NAV1)
      {
        sprintf(buffer, "nav1s:%04d\n" , num); 
      }

      else if (active_bank_two == NAV2)
      {
        sprintf(buffer, "nav2s:%04d\n" , num); 
      }
      else if (active_bank_two == COM1)
      {
        sprintf(buffer, "com1s:%04d\n" , num); 
      }
      else if (active_bank_two == COM2)
      {
        sprintf(buffer, "com2s:%04d\n" , num); 
      }
    }

    else if (strcmp(name,"heading")==0)
    {
      sprintf(buffer, "heading:%d\n" , val); 
    }

    else if (strcmp(name, "obs")==0)
    {
      if (active_obs == OBS1)
      {
        sprintf(buffer, "obs1:%d\n" , val); 
      }

      else if (active_obs == OBS2)
      {
        sprintf(buffer, "obs2:%d\n" , val); 
      }
    }
    else if (strcmp(name, "ap")==0)
    {
      if (active_altitude == ALTITUDE)
      {
        sprintf(buffer, "altitude:%d\n" , val); 
      }

      else if (active_altitude == VERTICAL)
      {
        sprintf(buffer, "vertical:%d\n" , val *100); 
      }
    }

    else if (strcmp(name, "adf")==0)
    {
      if (active_ADF == ADF1)
      {
        num = atoi(adf[active_ADF]);
        num += val; 
        sprintf(buffer, "adf1:%04d\n",  num); 
      }

      else if (active_ADF == ADF2)
      {
        num = atoi(adf[active_ADF]);
        num += val; 
        sprintf(buffer, "adf2:%04d\n",  num); 
      }

      else if (active_ADF == TRANSPONDER)
      {
        num = atoi(adf[active_ADF]);
        num+=val;
        sprintf(buffer, "trans:%04d\n" , num); 
      }
    }
    Serial.write(buffer); 
  }
}


void send_hex( char* name, int val, int len)
{
  char buffer[20];
  if (len == 1)
  {
    sprintf(buffer, "%s:0x%x\n" , name, val); 

  }

  else if (len == 2)
  {
    sprintf(buffer, "%s:0x%02x\n" ,name, val); 

  }

  else if (len == 3)
  {
    sprintf(buffer, "%s:%03x\n" ,name, val); 
  }

  else if (len == 4)
  {
    sprintf(buffer, "%s:%04x\n" ,name, val); 
  }
  Serial.write(buffer); 
}

void check_input ()
{
}

void toggle_button(AdvButton* but)
{
  send_string(but->getName(), "toggle");
}

void single_button_on(AdvButton *but)
{
  send_string(but->getName(), "on");
}

void single_button_off(AdvButton *but)
{
  send_string(but->getName(), "off");
}

void lights_on(AdvButton *but)
{
  but->setBit();
  send_hex ("lights", but->getBytes(), 2);
}

void change_step_bank_one(AdvButton* but)
{
  if (step_bank1 == 1)
  {
    step_bank1 =100; 
  }

  else
  {
    step_bank1 = 1; 
  }
}

void change_stepped_transponder(AdvButton* but)
{
  if (transponder_step == 1)
  {
    transponder_step = 10000; 
  }
  transponder_step /= 10; 
}

void swap_bank1(AdvButton* but)
{
  int  beats = 1 << active_bank_one; 
  send_hex("swap" , beats, 1);
}

void swap_bank2(AdvButton* but)
{
  int  beats = 1 << active_bank_two; 
  send_hex("swap" , beats, 1);
}

void lights_off(AdvButton *but)
{
  but->clearBit();
  send_hex ("lights", but->getBytes(), 2);
}

void select_bank1(AdvButton *but)
{
  active_bank_one= atoi(&but->getName()[3]);
  update_display(0); 
}

void select_bank2(AdvButton *but)
{
  active_bank_two= atoi(&but->getName()[3]);
  update_display(1); 
}

void select_adf(AdvButton *but)
{
  active_ADF = atoi(&but->getName()[3]);
  update_display(2); 
}

void select_obs(AdvButton *but)
{
  active_obs = atoi(&but->getName()[3]);
  update_display(3); 
}

void select_altitude (AdvButton *but)
{
  active_altitude = atoi(&but->getName()[3]);
  update_display(4); 
}

void send_magneto(AdvButton *but)
{
 int mcmd= atoi(&but->getName()[3]);
 send_value("magneto",mcmd);
}

void write_led_decimals(int display, int value)
{
  digitalWrite (display, LOW);
  delay(10);
  SPI.transfer(0x77);     // Decimal Point OpCode
  SPI.transfer(value);    // Decimal Point Values
  digitalWrite (display, HIGH); //release chip, signal end transfer
}

void reset7seg(int display)
{
  digitalWrite (display, LOW);
  delay(100);
  SPI.transfer(0x76);    // Decimal Point Values
  //SPI.transfer(0x0);
  digitalWrite (display, HIGH); //release chip, signal end transfer
}

void write_string (int display,char*  s) {
  write_led_numbers(display,(s [0]),(s [1]),(s [2]),(s [3]));
}  

void write_led_numbers(int display, char digit1, char digit2, char digit3, char digit4)
{
  digitalWrite (display, LOW);
  delay(10);
  SPI.transfer(digit1);    // Thousands Digit
  delay(10);
  SPI.transfer(digit2);    // Hundreds Digit
  delay(10);
  SPI.transfer(digit3);    // Tens Digit
  delay(10);
  SPI.transfer(digit4);    // Ones Digit
  digitalWrite (display, HIGH); //release chip, signal end transfer

}
void write_led(int display, unsigned short num, unsigned short base, unsigned short pad)
{
  unsigned short digit[4] = { 
    0, ' ', ' ', ' '                                   };
  unsigned short place = 0;

  if ( (base<2) || (base>16) || (num>(base*base*base*base-1)) ) {
    write_led_numbers(display,' ', 0x00, 0x0f, ' ');  // indicate overflow
  } 
  else {
    while ( (num || pad) && (place<4) ) {
      if ( (num>0)  || pad )
        digit[place++] = num % base;
      num /= base;
    }
    write_led_numbers(display, digit[3], digit[2], digit[1], digit[0]);
  }
}








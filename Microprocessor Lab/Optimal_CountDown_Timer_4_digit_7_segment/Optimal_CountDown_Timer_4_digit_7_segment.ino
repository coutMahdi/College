//CountDown Timer 4-digit 7-segment
//use pin 0 & 1 for Serial Communication and Serial read to set start_time_4digit
//just initialize start_time and delay_to_display_every_digit
//in proteus: 1 <= delay_to_display_every_digit <= 16  0 super changing bc without delay: you cannot recognize any number) - 15 without any blink 16 ok - 17 sometimes - 20 many
const size_t delay_to_display_every_digit = 10, delay_to_display_4_digit = 4 * delay_to_display_every_digit + 8;//8 is based on try and error (actually 7.45)
const byte second_coefficient = 1000 / delay_to_display_4_digit;//20
// const size_t remaining_millisec = 1000 - (second_coefficient * delay_to_display_4_digit);//paresh hengame taghyeer
const size_t start_time_4digit = 2002;
const size_t start_minute = start_time_4digit / 100, start_second = start_time_4digit % 100;

byte digit[4];

void show_digit(byte);
void show_4_digit();
void select_digit(byte);
void deselect_digit();
void display_one_second(short int, short int);
void display_one_minute(short int, short int);
void display_one_minute(short int);
void digitize_minutes(short int);
void digitize_seconds(short int);

void setup() {
  //to reserve pin 0 & 1 for serial communication start from pin 2
  for (size_t pin_NO = 2; pin_NO <= 7; pin_NO++)
    pinMode(pin_NO, OUTPUT);
  for (size_t pin_NO = 2; pin_NO <= 3; pin_NO++)
    digitalWrite(pin_NO, HIGH);
}

void loop() {
  //First_Incomplete_Minute_CountDown();
  display_one_minute(start_minute, start_second);
  //Other_Minutes_CountDown()
  for (short int minute = start_minute - 1; minute >= 0; minute--){
    display_one_minute(minute);
  }
}

//----------------------------------------------------------- 7-segment display functions (how many? 4 & one overloaded once = 5) ------------------------------------------------------------
void show_digit(byte BCD_digit){//Wrties inputs of 7448 ABCD 
  digitalWrite(7, (BCD_digit & 0b0001) ? HIGH : LOW);//A
  digitalWrite(6, (BCD_digit & 0b0010) ? HIGH : LOW);//B
  digitalWrite(5, (BCD_digit & 0b0100) ? HIGH : LOW);//C
  digitalWrite(4, (BCD_digit & 0b1000) ? HIGH : LOW);//D
}
void show_4_digit(){
  for(byte digit_NO = 0; digit_NO < 4; digit_NO++){
    select_digit(digit_NO);
    show_digit(digit[digit_NO]);
    delay(delay_to_display_every_digit);
    deselect_digit();
  }
}
//------modules for 2 to 4 decoder 4555
void select_digit(byte digit_NO){//Note that selector is Active-Low
  digitalWrite(3, (digit_NO & 0b01) ? LOW : HIGH);
  digitalWrite(2, (digit_NO & 0b10) ? LOW : HIGH);
}
void deselect_digit(){//just toggle not bitwise and again
  digitalWrite(3, !digitalRead(3));
  digitalWrite(2, !digitalRead(2));
}
//---
void display_one_second(short int minute, short int second){
  // size_t recent_millis = millis();//approach 2 less accurate (1000 - delay_to_display_4_digit / 2 >= millis() - recent_millis)
  //this loop approximate one second and ensures being less than one second
  for(size_t i = 0; i < second_coefficient; i++)
    show_4_digit();
  //Regulate number of loops to Simulate one second using millis() built-in timer function
  size_t expected_seconds = start_second + (start_minute - minute) * 60 - second;
  for(size_t i = 0; (expected_seconds + 1) * 1000 - delay_to_display_4_digit / 2 >= millis(); i++)
    show_4_digit();
}
void display_one_minute(short int minute, short int starting_second){//min: 0 , max: 59
  digitize_minutes(minute);
    for (short int second = starting_second; second >= 0; second--){
      digitize_seconds(second);
      display_one_second(minute, second);
    }
}
void display_one_minute(short int minute){
  display_one_minute(minute, 59);
}
//----------------------------------------------------------- two math function (how many? 2) ------------------------------------------------------------
void digitize_minutes(short int minute){
  digit[2] = minute % 10;
  digit[3] = minute / 10;
}
void digitize_seconds(short int second){
  digit[0] = second % 10;
  digit[1] = second / 10; 
}

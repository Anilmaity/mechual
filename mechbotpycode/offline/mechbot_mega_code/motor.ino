void motor_setup()
{
   rmcs.Serial_selection(0);       //Serial port selection:0-Hardware serial,1-Software serial
   //rmcs.Serial(9600);
   Serial.println("RMCS-2303 Position control mode demo\r\n\r\n");

   rmcs.begin(&Serial1,9600);    //Uncomment if using hardware serial port for mega2560:Serial1,Serial2,Serial3 and set baudrate. Comment this line if Software serial port is in use
   //rmcs.begin(&myserial,9600);     //Uncomment if using software serial port. Comment this line if using hardware serial.
   rmcs.WRITE_PARAMETER(slave_id,INP_CONTROL_MODE,PP_gain,PI_gain,VF_gain,LPR,acceleration,speed);    //Uncomment to write parameters to drive. Comment to ignore.
   rmcs.READ_PARAMETER(slave_id);
   
}

void motor_loop()
{
   Serial.println("Sending absolute position command to -50000");
   rmcs.Absolute_position(slave_id,-50000);   //enter position count with direction (CW:+ve,CCW:-ve) 
   
   while(1)       //Keep reading positions. Exit when reached.
   {
      Current_position=rmcs.Position_Feedback(slave_id); //Read current encoder position 
      Current_Speed=rmcs.Speed_Feedback(slave_id);       //Read current speed
      Serial.print("Position Feedback :\t");
      Serial.print(Current_position);
      Serial.print("\t\tSpeed Feedback :\t");
      Serial.println(Current_Speed);

      delay(100);
      if(Current_position==-50000)
      {
         Serial.println("Position -50000 reached.");
         break;
      }
   }
   delay(2000);
   
   
   Serial.println("Sending absolute position command to 50000");
   rmcs.Absolute_position(slave_id,50000);   //enter position count with direction (CW:+ve,CCW:-ve) 
   
   while(1)       //Keep reading positions. Exit when reached.
   {
      Current_position=rmcs.Position_Feedback(slave_id); //Read current encoder position 
      Current_Speed=rmcs.Speed_Feedback(slave_id);       //Read current speed
      Serial.print("Position Feedback :\t");
      Serial.print(Current_position);
      Serial.print("\t\tSpeed Feedback :\t");
      Serial.println(Current_Speed);
      delay(100);
      if(Current_position==50000)
      {
         Serial.println("Position 50000 reached.");
         break;
      }
   }
   delay(2000);

   
   Serial.println("Disabling motor.");
   rmcs.Disable_Position_Mode(slave_id);            //Disable postion control mode
   delay(1000);
}
 

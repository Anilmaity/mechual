void motor_setup()
{
   rmcs.Serial_selection(0);       //Serial port selection:0-Hardware serial,1-Software serial
   //rmcs.Serial(9600);
   Serial.println("RMCS-2303 Position control mode demo\r\n\r\n");

   rmcs.begin(&Serial2,9600);    //Uncomment if using hardware serial port for mega2560:Serial1,Serial2,Serial3 and set baudrate. Comment this line if Software serial port is in use
   //rmcs.begin(&myserial,9600);     //Uncomment if using software serial port. Comment this line if using hardware serial.
   rmcs.WRITE_PARAMETER(slave_id,INP_CONTROL_MODE,PP_gain,PI_gain,VF_gain,LPR,acceleration,speed);    //Uncomment to write parameters to drive. Comment to ignore.
   rmcs.READ_PARAMETER(slave_id);
   
}

void motor_loop()
{
  int response =  rmcs.Absolute_position(slave_id,Sterring_input);   //enter position count with direction (CW:+ve,CCW:-ve) 
    if(response == 1){
      M_D_connected = true;
      Current_position=rmcs.Position_Feedback(slave_id);
    }
    else{
      M_D_connected = false;
       
    }
 
}
 

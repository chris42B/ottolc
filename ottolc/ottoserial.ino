// this file manages serial communications and command dispatch for debug&adjustment purposes
#include <stdio.h>
bool isDebug(){
  beep();
  return true;
  delay(2000);
  return Serial.available();
}

int testvars[10] = {0,0,0,0,0,0,0,0,0,0};

void setServo(EServo servo, int value);
void replSetServo(String args){
  Serial.println("setting a servo");
  int servonr;
  int degree;
  int matches = sscanf(args.c_str(), "%d %d", &servo, &degree);
  if(matches!=2){
    Serial.print(matches);
    Serial.print("could not parse arguments: '");
    Serial.print(args);
    Serial.println("'");
    return;
  }
  EServo servo;
  switch(servonr){
    case 0:
      servo = rightFoot; break;
    case 1:
      servo = leftFoot; break;
    case 2:
      servo = rightLeg; break;
    case 3:
      servo = leftLeg; break;
    default:
      Serial.print("Unknknown servo nr ");
      Serial.println(servonr, DEC);
    break;
  }
  setServo(servo, degree);
  Serial.println("Ok");
}

void setTVar(String args){
  Serial.println("setting testvar");
  int varnr;
  int value;
  int matches = sscanf(args.c_str(), "%d %d", &varnr, &value);
  if(matches!=2){
    Serial.print(matches);
    Serial.print("could not parse arguments: '");
    Serial.print(args);
    Serial.println("'");
    return;
  }
  testvars[varnr] = value;
  Serial.println("Ok");
}


void replSetTrim(String args){
  Serial.println("setting servo trim");
  int servonr=-1;
  int trimvalue=-1;
  int matches = sscanf(args.c_str(), "%d %d", &servonr, &trimvalue);
  if(matches!=2){
    Serial.print(matches);
    Serial.print("could not parse arguments: '");
    Serial.print(args);
    Serial.println("'");
    return;
  }
  Serial.print("setting servo to: ");
  Serial.print(servonr);
  Serial.println(trimvalue);
  EServo servo;
  switch(servonr){
    case 0:
      servo = rightFoot; break;
    case 1:
      servo = leftFoot; break;
    case 2:
      servo = rightLeg; break;
    case 3:
      servo = leftLeg; break;
    default:
      Serial.print("Unknknown servo nr ");
      Serial.println(servonr, DEC);
    break;
  }
  setTrimdata(servo, trimvalue);
  Serial.println("Ok");
}

void replTrimtest(){
  for(int i=0;i<4;i++){
    servo[i].SetPosition(60);
  }
  delay(500);
  resetServos();
}

void doCommands(){
  digitalWrite(13, HIGH);
  Serial.println("Hello my name is otto, version 0.0.1");
  Serial.readStringUntil('\n'); // Clear serial buffer
  Serial.setTimeout(5l*60l*1000l);
  while(true){
    Serial.print("enter command: "); 
    String line = Serial.readStringUntil('\n');
    if(line.length()==0){
      Serial.println();
      continue;
    }
    auto pos = line.indexOf(' ');
    if(pos<0){
      pos = line.length();
    }
    String command = line.substring(0, pos);
    Serial.print(command + " ");
    String args = "";
    if( (pos+1)<(line.length()) ){
      args = line.substring(pos+1, line.length());
      Serial.println(args);
    }


    //TODO: replace with something better, hash map?
    if(command=="ping"){
      Serial.println("pong!");
    }else if(command=="setservo"){
      replSetServo(args);
    }else if(command=="resetservos"){
      resetServos();
      Serial.println("servos reseted");
    }else if(command=="beep"){
      beep();
    }else if(command=="trimtest"){
      replTrimtest();
    }else if(command=="settrim"){
      replSetTrim(args);
      replTrimtest();
    }else if(command=="dancetest"){
      // dancetest();
    }else if(command=="s"){
      setTVar(args);
    }else{
      Serial.print("command not found: ");
      Serial.println(command);
      beep();
    }
    
  }
}
 

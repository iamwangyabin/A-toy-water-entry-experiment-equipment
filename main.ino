const int releasePin = 13; // 释放机械手的继电器引脚
const int positionPin = 2; // 行程开关接收信号引脚
const int moter = 8;       // 控制电机启动关闭引脚

int incomingByte;              // 串口数据存储
int kflag='K';                 // 判断位
int qflag='K';
void setup() {
  // 初始化串口连接:
  Serial.begin(9600);
  // 以OUTPUT模式初始化release继电器:
  pinMode(releasePin, OUTPUT);
  // 以OUTPUT模式初始化电机控制引脚;
  pinMode(moter, OUTPUT);
  // 把行程开关输入端设置成拉上电阻状态下的输入，这样就不需要接上电阻了--原因见电工学;
  pinMode(positionPin, INPUT_PULLUP);
  while(Serial.read()>= 0){}///clear serialbuffer  

}

void loop() {
    int sensorVal=digitalRead(positionPin);
    if(kflag=='K'){
        while(Serial.read()>= 0){}///clear serialbuffer  
        Serial.print("=====================================\n");
        Serial.print("||         等待输入控制命令：      ||\n");
        Serial.print("||输入H则会在倒计时结束开始运行电机||\n");
        Serial.print("||      输入J则会让机械手张开      ||\n");
        Serial.print("||      输入K则会让机械手闭合      ||\n");
        Serial.print("||     请注意不要输入其他指令      ||\n");
        Serial.print("||      指令顺序一般为J->K->H      ||\n");
        Serial.print("=====================================\n");
        kflag='Q';
    }
    //读取数据流中最后一个字符:
    incomingByte = Serial.read();
    while ( incomingByte != -1 ){
        if( qflag=='K' && incomingByte== 'H'){
            Serial.print("=====================================\n");
            Serial.print("||****************5****************||\n");
            delay(1000);
            Serial.print("||****************4****************||\n");
            delay(1000);
            Serial.print("||****************3****************||\n");
            delay(1000);
            Serial.print("||****************2****************||\n");
            delay(1000);
            Serial.print("||****************1****************||\n");
            delay(1000);
            Serial.print("=====================================\n");
            qflag='Q';
        }
        int sensorVal=digitalRead(positionPin);
        // 如果时字母H(ASCII码72), 开始
        if (incomingByte == 'H') {
            if (sensorVal==LOW){
                Serial.write("小车正在向前运行，等待到位释放\n");
                digitalWrite(releasePin, HIGH);
                digitalWrite(moter, LOW);
            }
            else{
                Serial.write("到释放位置\n");
                digitalWrite(releasePin, LOW); // 让机械手释放，有可能时相反的就是继电器上电线接错了。
                delay(500); // 等待0.5秒再停电机
                Serial.print("电机停止运转\n");
                digitalWrite(moter, HIGH);      // 电机停止运转--这个比较关键，有可能会损坏仪器
                kflag='K';
                qflag='K';
                break;
            }
        }
        if(incomingByte == 'J'){
            digitalWrite(releasePin, HIGH);
            int i=incomingByte;
            i = Serial.read();
            if(i!=-1){
                incomingByte=i;
            }
        }
        if(incomingByte == 'K'){
            digitalWrite(releasePin, LOW);
            int i=incomingByte;
            i = Serial.read();
            if(i!=-1){
                incomingByte=i;
            }
        }
    }
}

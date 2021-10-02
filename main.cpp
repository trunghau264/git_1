#include <Arduino.h>
//=========================================KHAI BÁO THU VIỆN=============================
  #include <DS1302.h>
  #include <Wire.h>
  #include<LiquidCrystal_I2C.h>
  #include"DHT.h"
  #include <SoftwareSerial.h>                         // khai báo thư viện cho sim900a
  SoftwareSerial SIM900A(5, 6);                      // khai báo chân TX,RX
  //_____________cấu hình lcd_________// i2c=0x27 lcd= 16.2
  LiquidCrystal_I2C lcd(0x27,16,2); 
  //_______ khai báo các chân của DS1302__________//
  
  DS1302 rtc(2, 3, 4);//rst.dat.clk
  
  //________khai báo chân của DHT11_______________//
  
  int RELAY= 8;                                       // khai báo chân điều khiển
  String CONTENT;                                     // khai báo biến có dữ liệu là String(kiễu dữ liệu CHUỖI)                         
  Time t; 
  DHT dht;
void setup()
{
//==============================KHỞI TẠO===================================================================//
                           
  delay(2000);                    // tạo độ trể
  rtc.halt(false);                //thư viện của DS1302 chân rtc
  rtc.writeProtect(false);        //thư viện của DS1302 chân rtc
  
  //__________________-_cài đặt thời gian cho DS1302_______________//
 rtc.setTime(12, 52, 00);                               //24h
  rtc.setDate(29, 9, 2021);                              //NGAY_THANG_NAM SETUP
  lcd.init();                                           // KHOI TAO LCD
  lcd.backlight();                                      // bật đèn lcd
  lcd.setCursor(0,0);                                   // cột1,hàng1
  pinMode(RELAY, OUTPUT);                               //gán chân số 7 là chân Output
  digitalWrite(RELAY,HIGH);                             // gán chân số 7 là chân Input
  Serial.begin(9600);                                   // tạo kết nối 9600 bit mỗi giây
  SIM900A.begin(9600);                                  //đồng bộ hóa với Sim900a
  delay(5000);                                          // tạo khoảng trễ là 5s
  Serial.print("SIM900A Home Automation Project is Done, Start Send AT Command"); // hiển thị trong Serial
  SIM900A.print("AT+CMGF=1\r");                         // ĐƯA TIN NHẮN VỀ DẠNG TEXT ĐỂ ĐỌC TIN NHẮN
  delay(100);   
  dht.setup(7);
}

//===================================CHUONG TRINH CON GUI TIN NHAN==========================//
void SendMessage()
{
  SIM900A.println("AT+CMGF=1");                     //Khởi động chế độ văn bản
  delay(1000);                                      //tạo độ trễ
  SIM900A.println("AT+CMGS=\"+84395974621\"\r");    //số di động người nhận
  delay(1000);                                      //tạo độ trễ
  SIM900A.println("I LOVE YOU");                    //nội dung tin nhắn
  delay(100);                                       //tạo độ trễ
  SIM900A.println((char)26);                        //Hoàn thành gửi
  delay(1000);                                      //tạo độ trễ
}
void SIM()
{
 if (SIM900A.available() > 0)                       // nếu có tín hiệu lớn hơn 0 {    
  {
  CONTENT = SIM900A.readString();                   // đọc biến Message
  Serial.print(CONTENT);                            //Hiển thị biến trên Serial
  delay(10);                                        //Tạo một khoảng thời gian trễ
  }
//==============================XỬ LÝ TÍN HIỆU ĐỂ ĐIỀU KHIỂN======================

  if (CONTENT.indexOf("OFF") >= 0)                  // nếu cái biến Mesage có dữ liệu là off
  {                
  digitalWrite(RELAY, LOW);                         // thì biến Bulb là chân điều khiển (7) relay xuất ra tín hiệu cao ( tắt relay)
  CONTENT = "";                                     // gán biến Message một khoảng trống
  }
  else if (CONTENT.indexOf("ON") >= 0)              // nếu tín hiệu Message có dữ liệu là on
  {            
  digitalWrite(RELAY, HIGH);                        //thì biến Bulb là chân điều khiển (7) relay xuất ra tín hiệu thấp ( mở relay)
  CONTENT = "";                                     // gán biến Message một khoảng trống
  }
  else if(CONTENT.indexOf("CALL")>=0)               //nếu tín hiệu Message có dữ liệu là CALL
  {
    SIM900A.println("ATD+84395974621;");            // gọi tới số điện thoại
    digitalWrite(RELAY, HIGH);                      // đèn sáng
    delay(10000);                                   // tạo đỗ trễ để thực thi cuộc gọi
    SIM900A.println("ATH");                         //CÚP MÁY
    SIM900A.println();                              // KHOẢNG TRÁNG ĐỂ REST
    CONTENT="";                                     // KHOẢNG TRÁNG ĐỂ REST
  }
  else if(CONTENT.indexOf("SMS")>=0)                //nếu tín hiệu Message có dữ liệu là SMS
  {
   SendMessage();                                   // CHẠY CHƯƠNG TRÌNH CON GỬI TIN NHẮN
   CONTENT="";                                      // KHOẢNG TRẮNG ĐỂ RESET
  }
} 
 void DOC_SIM()                                     //CHUONG_TRINH_CON DOC NHUYEN
{
  int c=0;                                          // KHOI TAO BIEN C                                           
  for (c;c<800;c++)                                 // LAP LAI KHOI LEN SIM 800 LAN
    {
       SIM();                                       // CHUONG TRINH CON SIM
    }
}
void loop() 
{
  delay(dht.getMinimumSamplingPeriod());

  int do_am = dht.getHumidity();
  int nhiet_do = dht.getTemperature();
  //===============================NGÀY THÁNG NĂM=================================//
  DOC_SIM();
  t = rtc.getTime();                                      //ĐẶT BIẾN t để lấy dữ liệu 
  DOC_SIM();
  
  DOC_SIM();                   
  
  DOC_SIM();
  lcd.setCursor(0,0);                                     // cột1,hàng1  
  DOC_SIM();
  lcd.print(t.date);                                      //NGAY
  DOC_SIM();
  lcd.print(":");     
  DOC_SIM();
  lcd.print(t.mon);                                       //THANG
  DOC_SIM();
  lcd.print(":");
  DOC_SIM();
  lcd.print(t.year);                                      //NAM
  DOC_SIM();
  lcd.setCursor(8,1);
  DOC_SIM();
  lcd.print("|");           
  DOC_SIM();
  lcd.print("DA");                                        //DO _AM
  DOC_SIM();
  lcd.print(":");
  DOC_SIM();
  lcd.print(do_am);                                       // lệnh làm tròn round
  DOC_SIM();
  lcd.print("%");                                          //%
  DOC_SIM();
  DOC_SIM();
  
//===============================GIỜ PHÚT GIÂY=================================//
    
  lcd.setCursor(0,1);                               // cột1,hàng2
  DOC_SIM();
  lcd.print(t.hour);                                // GIO
  DOC_SIM();
  lcd.print(":");
  DOC_SIM();   
  lcd.print(t.min);                                 // PHUT
  DOC_SIM();
  lcd.print(":");
  DOC_SIM(); 
  lcd.print(t.sec);                                 // GIAY
  DOC_SIM();
  lcd.setCursor(8,0);
  DOC_SIM();
  lcd.print("|");
  DOC_SIM();
  lcd.print("0c");
  DOC_SIM();
  lcd.print(":");
  DOC_SIM();
  lcd.print(nhiet_do);                              // BIEN NHIET DO DHT11
  DOC_SIM();
  lcd.print("C");                                   // CHU c
  DOC_SIM();
}

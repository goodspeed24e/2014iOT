int MIC_PIN = A0;

void setup()
{
  Serial.begin(9600);
  pinMode(MIC_PIN, INPUT);     
}

int count = 0; // 計算收到幾次的高音量

void loop()
{
  int mic = analogRead(MIC_PIN); // 讀取麥克風輸出
  
  if(mic >= 500){ // 這裡寫死500，您可試試別的值
    count++;
    Serial.print("mic >= 500, count=");
    Serial.println(count);
  }
}

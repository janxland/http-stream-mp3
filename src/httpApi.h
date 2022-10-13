#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
WiFiClient client;
HTTPClient http;
struct music{
    String id;
    String name;  
    String author;
    String src;
};
music lists[10];


String GET(String path)
{
  http.begin(client,path);
  http.addHeader("content-type", "application/x-www-form-urlencoded");
  int httpCode = http.GET();

  if (httpCode < 0) {
    Serial.println("request error - " + httpCode);
    return "error";

  }

  if (httpCode != HTTP_CODE_OK) {
    return "NOT OK";
  }
  String response =  http.getString();
  http.end();
  return response;
}
String POST(String path,String body)
{
  http.begin(client,path);
  http.addHeader("content-type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(body);

  if (httpCode < 0) {
    Serial.println("request error - " + httpCode);
    return "error";

  }

  if (httpCode != HTTP_CODE_OK) {
    return "NOT OK";
  }
  String response =  http.getString();
  http.end();
  Serial.println(response);
  return response;
}

String getUrl(String id){

  DynamicJsonDocument doc(4096);
  Serial.println("types=url&source=netease&id="+id);
  deserializeJson(doc, POST("http://music.ytxmgy.com/api.php","types=url&source=netease&id="+id));
  JsonObject list = doc.as<JsonObject>();
  return list["url"].as<String>();
}
void pushMusic(String key,int page){
  DynamicJsonDocument doc(4096);
  deserializeJson(doc, POST("http://music.ytxmgy.com/api.php","types=search&count=10&source=netease&pages=1&name="+key));
  JsonArray list = doc.as<JsonArray>();
  for(int i=0;i<list.size();i++){
    lists[i].id = list[i]["id"].as<String>();
    lists[i].name = list[i]["name"].as<String>();
    lists[i].author = list[i]["artist"][0].as<String>();
  }
  // String body = "_p=163&id=";
  // for(int i=0;i<10;i++){
  //   if(i==19){
  //     body=body+lists[i].id;
  //   }else{
  //     body=body+lists[i].id+",";
  //   }
  // }
  // DynamicJsonDocument doc1(5120);
  // deserializeJson(doc1, POST("http://music.jsososo.com/apiMix/url/batch",body));
  // for(int i=0;i<10;i++){
  //   char str[10];
  //   scanf("%d", lists[i].id);
  //   itoa(lists[i].id, str, 10);
  //   lists[i].src = doc1["data"][str].as<String>().c_str();
  // }
}
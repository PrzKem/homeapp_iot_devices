void post_request(String url, String payload)
{
  String contentType = "application/x-www-form-urlencoded";
  if(!semaphore_requestBlock)
  {
    semaphore_requestBlock = true;
    Serial.println("making POST request");
    client.post(url, contentType, payload);
    read_response();
  }
  else
  {
    Serial.println("POST Blocked by semaphore");
  }
}

void post_keep_alive(int deviceID, String name, String location)
{
  Serial.println("Sending keep alive");
  String dataToSend = "name="+name+"&";
  dataToSend += "location="+location+"&";
  dataToSend += "id="+String(deviceID)+"&";
  dataToSend += String("actual_work_mode=")+String(actualWorkMode==1?"auto":"manu");
  Serial.println(dataToSend);
  post_request("/api/controllers", dataToSend);
}


void post_sensor_reading(String url, int sensor_id, float value)
{
  Serial.println("Sending sensor reading");
  String sensor = "sensor_id="+String(sensor_id)+"&";
  String postData = "reading_value="+String(value);
  post_request(url, sensor+postData);
}

void get_request(String url, int token_id)
{
  String response;
  if(!semaphore_requestBlock)
  {
    semaphore_requestBlock = true;
    Serial.println("making GET request");
    client.beginRequest();
    client.get(url);
    client.sendHeader("token_val", String(token_id));
    client.endRequest();
    response = read_response();

    switch(token_id)
    {
      case 1:
        actualWorkMode=(response=="1"?1:2);
        Serial.println("Actual work mode="+String(actualWorkMode));
        break;
      default:
        Serial.print("unused token");
        break;        
    }
            
  }
  else
  {
    Serial.println("GET Blocked by semaphore");
  }
}

String read_response()
{
  if(semaphore_requestBlock)
  {
    semaphore_requestBlock = false;
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();

    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
    return response;
  }
  else
  {
    Serial.println("Response doesn't have semaphore");
  }
}
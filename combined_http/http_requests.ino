void post_request(String url, int sensor_id, float value)
{
  if(!semaphore_requestBlock)
  {
    semaphore_requestBlock = true;
    Serial.println("making POST request");
    String contentType = "application/x-www-form-urlencoded";
    String sensor = "sensor_id="+String(sensor_id)+"&";
    String postData = "reading_value="+String(value);

    client.post(url, contentType, sensor+postData);
    read_response();
  }
  else
  {
    Serial.println("POST Blocked by semaphore");
  }
}

void get_request(String url, int token_id)
{
  if(!semaphore_requestBlock)
  {
    semaphore_requestBlock = true;
    Serial.println("making GET request");
    client.beginRequest();
    client.get(url);
    client.sendHeader("token_val", String(token_id));
    client.endRequest();
    read_response();
  }
  else
  {
    Serial.println("GET Blocked by semaphore");
  }
}

void read_response()
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
  }
  else
  {
    Serial.println("Response doesn't have semaphore");
  }
}
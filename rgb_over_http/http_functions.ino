void put_request(String url, int sensor_id, float value)
{
  if(!semaphore_requestBlock)
  {
    semaphore_requestBlock = true;
    Serial.println("making PUT request");
    String contentType = "application/x-www-form-urlencoded";
    String token = "&token_val=1";
    String sensor = "sensor_id="+String(sensor_id)+"&";
    String putData = "reading_value="+String(value);

    client.put(url, contentType, sensor+putData+token);
    read_response();
  }
  else
  {
    Serial.println("PUT Blocked by semaphore");
  }
}

String get_request(String url, int token_id)
{
  if(!semaphore_requestBlock)
  {
    semaphore_requestBlock = true;
    Serial.println("making GET request");
    client.beginRequest();
    client.get(url);
    client.sendHeader("token_val", String(token_id));
    client.endRequest();
    return read_response();
  }
  else
  {
    Serial.println("GET Blocked by semaphore");
  }
  return "";
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
  return "";
}
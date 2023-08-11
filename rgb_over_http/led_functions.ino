void set_monocolour(RGB values)
{
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(values.r, values.g, values.b));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(1); // Pause before next pass through loop
  }
}
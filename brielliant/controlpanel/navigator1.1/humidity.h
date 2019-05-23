#ifndef humidity_h
#define humidity_h

#define DHT_PIN A3 
#define DHT_TYPE DHT11 

// blow level constant, can be increased/decreased 
// depending on how much player should blow 
#define HUM_COMPARE 2

// for some reason, the dht11 needs a few reads WITH a few ms
// in between to clear out its old readings ...
#define HUM_WAIT_MS 5
#define HUM_NUM_READS 400

void humSetup(bool mode);

bool humCheck();

void humSetBasePt();

#endif

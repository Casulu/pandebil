#ifndef SRF02_H_
#define SRF02_H_

#define SRF_FRONT 0xE0
#define SRF_BACK 0xEA

void srf02_trigger(char srf02);
int srf02_read(char srf02);
bool srf02_timer_alarm();
void srf02_timer_reset();
void srf02_timer_tick();

#endif /* SRF02_H_ */
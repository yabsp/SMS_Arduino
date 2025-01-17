The SIM7600G-H Module might get stuck in some buggy firmware. It will not be able to connect to network automatically on its own anymore. To resolve this problem one has to use:
*AT+CFUN=0 (set functionality to reduced, disables network capability)
*AT+CFUN=1 (set functionality to full, enables network capability)
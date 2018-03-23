# Banking Algorithm implementation 
**For n Customers and m resources, this program determines whether any request
  from a customer leads the system into an unsafe or a safe state. This is a deadlock
  prevention algorithm that can be used for any application where deadlock handling must
  be done by the programmer.**
___
### To compile:
* ```make``` to compile
* ```make clean``` for cleanup

### To run:
* run as ```./bank 12 32 42``` where the CL arguments represent the available instance of 
each (physical/logical) resource class  in the system. 

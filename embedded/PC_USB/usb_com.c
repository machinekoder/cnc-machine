/* Function to read and write the LPC1758 USB port */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <usb.h>

//#define DEBUG 1
#define BULK_OUT_EP 0x05
#define BULK_IN_EP 0x82
#define DL 2

usb_dev_handle *locate_eStickv2 (void);

int main (int argc,char **argv)
{
  struct usb_dev_handle *eStickv2_handle;
  int send_status;
  int rec_status;
  int transmit = 0;
  char send_data[20];
  char string[100];// = "set x 10\n";
  unsigned char receive_data[20];
  
  snprintf(string, 100, "%s\n", argv[1]);

  usb_init();
  usb_set_debug(2);
  if ((eStickv2_handle = locate_eStickv2())==0)
  {
    printf("Could not open the eStickv2 device\n");
    return (-1);
  }

  usb_set_configuration(eStickv2_handle, 1);

  usb_claim_interface(eStickv2_handle, 0);

  transmit = strlen(&string[0]);

  send_data[0]=0x00ff&((transmit+1)>>8);  //Highbyte
  send_data[1]=0x00ff&(transmit+1);     	//Lowbyte

  sprintf(&send_data[DL],"%s",&string[0]);

  printf("TX String = %s",&send_data[DL]);

  send_status = usb_bulk_write(eStickv2_handle,BULK_OUT_EP,&send_data[0],transmit+DL,10);

  send_status = send_status;				/* prevent compiler warning */

#ifdef DEBUG
  printf("TX stat=%d\n",send_status);
#endif

  do {
    rec_status = usb_bulk_read(eStickv2_handle,BULK_IN_EP,(char *)&receive_data[0],15+DL,10);
#ifdef DEBUG
    printf("RX stat=%d\n",rec_status);
#endif
  } while(rec_status != 15+DL);

  printf("RX String = %s",&receive_data[DL]);

  usb_close(eStickv2_handle);

  return 0;
}

usb_dev_handle *locate_eStickv2(void)
{
  unsigned char located = 0;
  struct usb_bus *bus;
  struct usb_device *dev;
  usb_dev_handle *device_handle = 0;

  usb_find_busses();
  usb_find_devices();

  for (bus = usb_busses; bus; bus = bus->next)
  {
    for (dev = bus->devices; dev; dev = dev->next)
    {
      if (dev->descriptor.idVendor == 0xfefe)
      {
        located++;
        device_handle = usb_open(dev);
        printf("eStickv2 Device found\n");
#ifdef DEBUG
        printf("eStickv2 Device Found @ Address %s \n", dev->filename);
        printf("eStickv2 Vendor ID 0x0%x\n",dev->descriptor.idVendor);
        printf("eStickv2 Product ID 0x0%x\n",dev->descriptor.idProduct);
#endif
      }
#ifdef DEBUG
      else printf("** usb device %s found **\n", dev->filename);
#endif
    }
  }

  if (device_handle==0) return (0);
  else return (device_handle);
}


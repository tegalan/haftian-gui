#include "listusbdrive.h"

extern "C" {
#include <libudev.h>
}

static struct udev_device*
get_child(struct udev* udev, struct udev_device* parent, const char* subsystem)
{
    struct udev_device* child = NULL;
    struct udev_enumerate *enumerate = udev_enumerate_new(udev);

    udev_enumerate_add_match_parent(enumerate, parent);
    udev_enumerate_add_match_subsystem(enumerate, subsystem);
    udev_enumerate_scan_devices(enumerate);

    struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry *entry;

    udev_list_entry_foreach(entry, devices) {
        const char *path = udev_list_entry_get_name(entry);
        child = udev_device_new_from_syspath(udev, path);
        break;
    }

    udev_enumerate_unref(enumerate);
    return child;
}


QStringList ListUsbDrive() {
    struct udev *_udev = udev_new();
    struct udev_enumerate* enumerate = udev_enumerate_new(_udev);
    QStringList result;

    udev_enumerate_add_match_subsystem(enumerate, "scsi");
    udev_enumerate_add_match_property(enumerate, "DEVTYPE", "scsi_device");
    udev_enumerate_scan_devices(enumerate);

    struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry *entry;

    udev_list_entry_foreach(entry, devices) {
        const char* path = udev_list_entry_get_name(entry);
        struct udev_device* scsi = udev_device_new_from_syspath(_udev, path);

        struct udev_device* block = get_child(_udev, scsi, "block");
        struct udev_device* scsi_disk = get_child(_udev, scsi, "scsi_disk");

        struct udev_device* usb
            = udev_device_get_parent_with_subsystem_devtype(scsi, "usb", "usb_device");

        if (block && scsi_disk && usb) {
            result.append(udev_device_get_devnode(block));
//            printf("block = %s\n", udev_device_get_devnode(block));
//            printf("block = %s, usb = %s:%s, scsi = %s\n",
//                   udev_device_get_devnode(block),
//                   udev_device_get_sysattr_value(usb, "idVendor"),
//                   udev_device_get_sysattr_value(usb, "idProduct"),
//                   udev_device_get_sysattr_value(scsi, "vendor"));
        }

        if (block) {
            udev_device_unref(block);
        }

        if (scsi_disk) {
            udev_device_unref(scsi_disk);
        }

        udev_device_unref(scsi);
    }

    udev_enumerate_unref(enumerate);

    return result;
}

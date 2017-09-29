/*
 * QEMU Common PCI Host bridge configuration data space access routines.
 *
 * Copyright (c) 2006 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* Worker routines for a PCI host controller that uses an {address,data}
   register pair to access PCI configuration space.  */

#ifndef PCI_HOST_H
#define PCI_HOST_H

#include "hw/sysbus.h"

/*
 * Structures for all PCI host bridges
 */

#define TYPE_PCI_HOST "pci-host"
#define PCI_HOST(obj)                                           \
    OBJECT_CHECK(PCIHost, (obj), TYPE_PCI_HOST)
#define PCI_HOST_CLASS(klass)                                   \
    OBJECT_CLASS_CHECK(PCIHostClass, (klass), TYPE_PCI_HOST)
#define PCI_HOST_GET_CLASS(obj)                                 \
    OBJECT_GET_CLASS(PCIHostClass, (obj), TYPE_PCI_HOST)

typedef struct PCIHost {
    Object parent;
} PCIHost;

typedef struct PCIHostClass {
    InterfaceClass parent;

    const char *(*root_bus_path)(PCIHost *, PCIBus *);
} PCIHostClass;

struct PCIHostState {
    MemoryRegion conf_mem;
    MemoryRegion data_mem;
    MemoryRegion mmcfg;
    uint32_t config_reg;
    PCIBus *bus;

    QLIST_ENTRY(PCIHostState) next;
};

/* common internal helpers for PCI/PCIe hosts, cut off overflows */
void pci_host_config_write_common(PCIDevice *pci_dev, uint32_t addr,
                                  uint32_t limit, uint32_t val, uint32_t len);
uint32_t pci_host_config_read_common(PCIDevice *pci_dev, uint32_t addr,
                                     uint32_t limit, uint32_t len);

void pci_data_write(PCIBus *s, uint32_t addr, uint32_t val, int len);
uint32_t pci_data_read(PCIBus *s, uint32_t addr, int len);

extern const MemoryRegionOps pci_host_conf_le_ops;
extern const MemoryRegionOps pci_host_conf_be_ops;
extern const MemoryRegionOps pci_host_data_le_ops;
extern const MemoryRegionOps pci_host_data_be_ops;

/*
 * Structures for the default SysBus based host bridge
 */

#define TYPE_SYSBUS_PCI_HOST "pci-host-bridge"
#define SYSBUS_PCI_HOST(obj) \
    OBJECT_CHECK(SysBusPCIHostState, (obj), TYPE_SYSBUS_PCI_HOST)

typedef struct SysBusPCIHostState {
    SysBusDevice busdev;

    PCIHostState phb;
} SysBusPCIHostState;

#define sysbus_pci_host_state(obj) (&SYSBUS_PCI_HOST(obj)->phb)

#endif /* PCI_HOST_H */

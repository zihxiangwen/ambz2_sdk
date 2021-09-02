# Matter (previously CHIP) on AmebaZ2

## How to support Matter on Ameba SDK

https://github.com/hank820/ambz2_sdk/blob/matter/MATTER_PORTING.md

## Get Ameba SDK & Matter SDK

    Test on Ubuntu 20.04

To check out this repository:

    git clone -b matter --recurse-submodules https://github.com/hank820/ambz2_sdk.git

If you already have a checkout, run the following command to sync submodules recursively:

	git submodule update --init --recursive


## Set Matter Build Environment 

    cd third_party/connectedhomeip

    source scripts/bootstrap.sh

    source scripts/activate.sh

    > Find more details to setup linux build environment
    > https://github.com/hank820/connectedhomeip/blob/master/docs/BUILDING.md


## Make CHIP library by gn and Make lib_main.a
### all-cluster-app

    cd ambz2_sdk/project/realtek_amebaz2_v0_example/GCC-RELEASE

    source ToolPath.sh
	
    make lib_all

### lighting-app

    cd ambz2_sdk/project/realtek_amebaz2_v0_example/GCC-RELEASE

    source ToolPath.sh

    make light

### CHIP core (generate by GN/ninja in connectedhomeip. Config by [lib_chip.mk](https://github.com/hank820/ambz2_sdk/blob/matter/project/realtek_amebaz2_v0_example/GCC-RELEASE/lib_chip.mk))

    output : ambz2_sdk/component/soc/realtek/8710c/misc/bsp/lib/common/GCC
	
    > libCHIP.a, ibCoreTests.a, ibChipCryptoTests.a, ibRawTransportTests.a...

### CHIP application (generate by [lib_chip_main.mk](https://github.com/hank820/ambz2_sdk/blob/matter/project/realtek_amebaz2_v0_example/GCC-RELEASE/lib_chip_main.mk)))

    output : ambz2_sdk/component/soc/realtek/8710c/misc/bsp/lib/common/GCC

    > lib_main.a

## Make Ameba application
    cd ambz2_sdk/project/realtek_amebaz2_v0_example/GCC-RELEASE

    make is

## Flash Image
    ambz2_sdk/project/realtek_amebaz2_v0_example/GCC-RELEASE/application_is/Debug/bin/flash_is.bin

## Run CHIP task on AmebaZ2 (all-cluster-app/lighting-app example)
### Bypass Rendezvous
* In "connectedhomeip/config/ambd/args.gni"
	* set `chip_bypass_rendezvous = true`
	* Set `chip_ip_commissioning = false`
	* Set `chip_use_clusters_for_ip_commissioning = false`
	* Set `chip_config_network_layer_ble = false`

* In "connectedhomeip/src/platform/AMBD/CHIPDevicePlatformConfig.h"
	* Set `#define CONFIG_USE_CLUSTERS_FOR_IP_COMMISSIONING	0`
	* Set `#define CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE 0`

* Build and Flash

### IP Commissioning
* In "connectedhomeip/config/ambd/args.gni"vim 
	* Set `chip_ip_commissioning = true`
	* Set `chip_use_clusters_for_ip_commissioning = true`
	* Set `chip_config_network_layer_ble = false`

* In "connectedhomeip/src/platform/AMBD/CHIPDevicePlatformConfig.h"
	* Set `#define CONFIG_USE_CLUSTERS_FOR_IP_COMMISSIONING	1`
	* Set `#define CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE 0`

* Build and Flash

### BLE Commissioning
* In "connectedhomeip/config/ambd/args.gni"
	* Set `chip_ip_commissioning = false`
	* Set `chip_use_clusters_for_ip_commissioning = false`
	* Set `chip_config_network_layer_ble = true`

* In "connectedhomeip/src/platform/AMBD/CHIPDevicePlatformConfig.h"
	* Set `#define CONFIG_USE_CLUSTERS_FOR_IP_COMMISSIONING	0`
	* Set `#define CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE 1`

* Build and Flash

## Test with [chip-tool](https://github.com/hank820/connectedhomeip/tree/master/examples/chip-tool)
Use standalone chip-tool app(linux) to communicate with the device.

In order to send commands to a device, it must be commissioned with the client. chip-tool currently only supports commissioning and remembering one device at a time. The configuration state is stored in `/tmp/chip_tool_config.ini`; deleting this and other `.ini` files in `/tmp` can sometimes resolve issues due to stale configuration.

### Bypass Rendezvous

* Enter the ATCMD `ATS$`
* Connect to AP using `ATW0, ATW1, ATWC` commands
* Run CHIP-TOOL IP commissioning command `./chip-tool pairing bypass XXX.XXX.XXX.XXX 5540 (Ameba IP)`

### IP Commissioning

* Enter the ATCMD `ATS$`
* Connect to AP using `ATW0, ATW1, ATWC` commands
* Run CHIP-TOOL IP commissioning command `./chip-tool pairing onnetwork 0 20202021 3840 XXX.XXX.XXX.XXX 5540 (Ameba IP)`

### BLE Commissioning

* Enter the ATCMD `ATS$`
* Run CHIP-TOOL IP commissioning command `./chip-tool pairing ble-wifi SSID PASSWORD 0 20202021 3840`

### Command for onoff cluster

Use PA_20 as output, connect a LED to this pin and GND.

* Run CHIP-TOOL on-off cluster command `./chip-tool onoff on 1`

* Run CHIP-TOOL on-off cluster command `./chip-tool onoff off 1`
    
### Command for doorlock cluster

Get doork lock cluster command on Ameba

* Run CHIP-TOOL on-off cluster command `./chip-tool doorlock lock-door 1 1`
    
* Run CHIP-TOOL on-off cluster command `./chip-tool doorlock unlock-door 1 1`

## Test with [Python Controller](https://github.com/hank820/connectedhomeip/blob/master/docs/guides/python_chip_controller_building.md)
To build the Python Controller (linux), run the following command.

	./scripts/build_python.sh --clusters_for_ip_commissioning true --chip_mdns platform

To launch Python Controller, activate the python environment first.
	
	source out/python_env/bin/activate
	chip-device-ctrl

### IP Commissioning
* Enter the ATCMD `ATS$`
* Connect to AP using `ATW0, ATW1, ATWC` commands
* Run python controller IP commissioning command `chip-device-ctrl > connect -ip <IP> 20202021 135246`
* Resolve DNS-SD name and update address of the node in the device controller. Get fabric ID using get-fabricid and use the decimal value of compressed fabric id. `chip-device-ctrl > get-fabricid`
* Resolve mDNS `chip-device-ctrl > resolve <Compressed Fabric ID> 135246`
* On-Off cluster command `chip-device-ctrl >zcl OnOff On 135246 1 1`
* On-Off cluster command `chip-device-ctrl >zcl OnOff Off 135246 1 1`

### BLE Commissioning
* Enter the ATCMD `ATS$`
* Run python controller BLE commissioning command `chip-device-ctrl > connect -ble 3840 20202021 135246`
* Provide network credentials `chip-device-ctrl > zcl NetworkCommissioning AddWiFiNetwork 135246 0 0 ssid=str:TESTSSID credentials=str:TESTPASSWD breadcrumb=0 timeoutMs=1000`
* Connect to AP `chip-device-ctrl > zcl NetworkCommissioning EnableNetwork 135246 0 0 networkID=str:TESTSSID breadcrumb=0 timeoutMs=1000`
* Close the BLE connection `chip-device-ctrl > close-ble`
* Resolve DNS-SD name and update address of the node in the device controller. Get fabric ID using get-fabricid and use the decimal value of compressed fabric id. `chip-device-ctrl > get-fabricid`

* Resolve mDNS `chip-device-ctrl >resolve <Compressed Fabric ID> 135246`
* On-Off cluster command `chip-device-ctrl >zcl OnOff On 135246 1 1`
* On-Off cluster command `chip-device-ctrl >zcl OnOff Off 135246 1 1`
rpmsg-tools
===========

- Used with: [Freertos relay-click example](https://github.com/texierp/freertos-warp7/blob/master/examples/imx7s_warp_m4/demo_apps/rpmsg/relay-click/relay-click.c);

# Examples

* RL1 ON

```console
$: rpmsg-test --RL1=1
received from cortex M4 = out_RL1:ok
```

* RL1 OFF

```console
$: rpmsg-test --RL1=0
received from cortex M4 = out_RL1:ok
```

* Wtite Slave ID to EEPROM

```console
$: rpmsg-test --out_eeprom=45
received from cortex M4 = out_eeprom:ok
```

* Read Slave ID from EEPROM

```console
$: rpmsg-test --in_eeprom
received from cortex M4 = in_eeprom:45
```

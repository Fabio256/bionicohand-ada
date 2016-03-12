# Firmware for the bionico dev kit

## Serial control

The hand received fingers positions through serial.

The serial command to control finger positions is as follows:

```
0,0,1,0,1,0,1,0,1,0.5,1;
```

The first '0' is constant. The next numbers are by pairs: position and
speed.

Each position and speed are constrained to the range 0-1.

For example, the command above sets the first finger at position 0 at
speed 1; The last finger is at position 0.5 and speed 1.

## Examples to copy-paste

Full open:

```

0,0,1,0,1,0,1,0,1,0,1;

```

Full close:

```

0,1,1,1,1,1,1,1,1,1,1;

```

Full close at variable speeds for each fingers:

```

0,1,0.1,1,0.2,1,0.4,1,0.6,1,0.9;

```

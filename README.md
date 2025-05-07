# RelaFind
Small program to relative search strings in a file. This can be useful to find a string in a binary file which might not use ASCII encoding.

## Usage
```
RelaFind <file> <needle> [flags]
```

### Flags

* `s`: Silent mode: display only occurrences.
* `c`: Count mode: display only the number of occurrences.

## Example
```
RelaFind "Soleil.md" "ESPADA"
```

It can't use wildcards, but can be used in a loop. For instance, in Windows:
```
for %f in (*.bin) do relafind %f "espada"
```

## License
Copyright (c) by Krusher, licensed under [GPLv3](LICENSE)
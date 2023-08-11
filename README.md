# Dale
A DNS filter written in C.

This project is intended to be used between a client and a DNS resolver
such as
[Unbound](https://nlnetlabs.nl/projects/unbound/about/).

The intended purpose for now is to filter DNS queries based on the QNAME (domain name).

This would allow you to control which domains you wish not to resolve,
for purposes such as ad/tracker blocking.

# Quick start
To install dale, run the following commands:
```
$ git clone https://github.com/jwren0/dale.git
$ cd dale/
# make install
```

# Make targets

## Building
```
$ make build
```

## Installing
```
# make install
```

## Uninstalling
```
# make uninstall
```

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

## Viewing usage
To view the usage for dale, you can run
```
$ dale --help
```

## Running
Assuming you have a resolver at 127.0.0.1 on port 8080, you
can run dale in the foreground using the following command:
```
$ dale -f -H 127.0.0.1 -P 8080
```

If you wish to run dale as a daemon, you can omit the "-f" option
or specify "-b".

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

# References
This project is based heavily on RFC 1035, along with
some aspects of RFC 3596 for handling IPv6 records (AAAA).

You can view these RFCs here:
- [RFC 1035](https://www.rfc-editor.org/rfc/rfc1035)
- [RFC 3596](https://www.rfc-editor.org/rfc/rfc3596)

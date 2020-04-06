s/_(print|scan)/_w\1/g
s/\b(_yIO_)?y(\w*)((scan|print|format)f?\()/\1y\2w\3/g
s/\bchar\b/wchar_t/g
s/\by((p|s)\w+\()/ywp\1/g
s/'([^']|\\x[[:xdigit:]]{2}|\\[0-9tvbafnr])'/L&/g
/include/s/yio_/ywio_/

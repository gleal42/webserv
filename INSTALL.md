# Installing necessary packages

## Mac OS

### CGI

You will need `php` installed on your local computer.

You can only install supported versions of PHP with brew. However, there is the tap `shivammathur/php` which can be used to install unsupported versions of PHP.

```bash
brew tap shivammathur/php
brew install shivammathur/php/php@7.3
brew link php@7.3
```

The first step only needs to be done once. After adding the tap, you can install PHP version 5.6 - 8.2.

Last step might need to be `brew link --overwrite php@7.3` instead if another version exists.

We use version **7.3** in this project.

Running `brew list | grep php` should return:

```bash
...
php@7.3
...
```

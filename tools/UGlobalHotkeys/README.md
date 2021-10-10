# UGlobalHotkey

A Qt extension library to provide global hotkey access across Windows, Mac, and Linux.

This project has been forked. The original readme below details that effort. This section defines changes and reasoning for these changes.

## Changes from ckaiser

* Resolves naming issues for Mac
  * `GetSimpleKeys` -> `getSimpleKeys`
  * `GetModifiers` -> `getModifiers`
* Removes qDebug notes for which keys are enabled.
* Adds support for Qt6 (keeps Qt5 support)

## Usage

Key combinations are set using plain text key names, separated by either `,` or `+`. Spacing between keys is not important. Modifier keys, such as `shift` are repsented by words. Note: not all modifier keys are supported.

### Modifier keys

| Key          | Name      | Alternate Name |
| ------------ | --------- | -------------- |
| Shift        | `shift`   | `shft`         |
| Control      | `control` | `ctrl`         |
| Alt          | `alt`     | --             |
| Windows/Meta | `meta`    | `win`          |

### Example

* `ctrl+shift+p`

## Original readme below

---

## Decription  

UGlobalHotkey is an extension for Qt framework, which implements global hotkeys functionality for Windows Linux and MacOSX platforms.
It is written by [bakwc](https://github.com/bakwc), extracted from [Pastexen](https://github.com/bakwc/Pastexen) and turned into a shared library by [falceeffect](https://github.com/falceeffect).

## Modifications in this fork

Mostly code style changes, better Windows support and whatever else I might need for [Lightscreen](http://github.com/ckaiser/Lightscreen).

## Building from source  

* You can either open project with QtCreator and press Build button
* Or build it using terminal:

``` sh
qmake  
make
```

## Usage example  

``` c++
UGlobalHotkeys *hotkeyManager = new UGlobalHotkeys();
hotkeyManager->registerHotkey("Ctrl+Shift+F12");
connect(hotkeyManager, &UGlobalHotkeys::activated, [=](size_t id)
{
    qDebug() << "Activated: " << QString::number(id);
});
```

## License

UGlobalHotkey library is licensed as Public Domain, so you are free to do anything with it.

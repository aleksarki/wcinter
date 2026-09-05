# WCI - Windows console interface

## [WCI](../wci.md) > Enumeration `Attribute`

Enumeration `Attribute` is based off type [`Word`](../types/word.md) and provides following options:

- `No` = `0`
---
- `ForegroundBlue` =      `0x01`: Text color contains blue.
- `ForegroundGreen` =     `0x02`: Text color contains green.
- `ForegroundRed` =       `0x04`: Text color contains red.
- `ForegroundIntensity` = `0x08`: Text color is intensified.
- `BackgroundBlue` =      `0x10`: Background color contains blue.
- `BackgroundGreen` =     `0x20`: Background color contains green.
- `BackgroundRed` =       `0x40`: Background color contains red.
- `BackgroundIntensity` = `0x80`: Background color is intensified.
---
- `CommonLvbLeadingByte` =    `0x0100`: Leading Byte of DBCS.
- `CommonLvbTrailingByte` =   `0x0200`: Trailing Byte of DBCS.
- `CommonLvbGridHorizontal` = `0x0400`: DBCS: Grid attribute: top horizontal.
- `CommonLvbGridLVertical` =  `0x0800`: DBCS: Grid attribute: left vertical.
- `CommonLvbGridRVertical` =  `0x1000`: DBCS: Grid attribute: right vertical.
- `CommonLvbReverseVideo` =   `0x4000`: DBCS: Reverse fore/back ground attribute.
- `CommonLvbUnderscore` =     `0x8000`: DBCS: Underscore.
- `CommonLvbSbcsDbcs` =       `0x0300`: SBCS or DBCS flag.
---
- `FgColorBlack` =   `0`:                                                 Text color is black.
- `FgColorBlue` =    `ForegroundBlue`:                                    Text color is blue.
- `FgColorGreen` =   `ForegroundGreen`:                                   Text color is green.
- `FgColorCyan` =    `ForegroundBlue  | ForegroundGreen`:                 Text color is cyan.
- `FgColorRed` =     `ForegroundRed`:                                     Text color is red.
- `FgColorMagenta` = `ForegroundBlue  | ForegroundRed`:                   Text color is magenta.
- `FgColorYellow` =  `ForegroundGreen | ForegroundRed`:                   Text color is yellow.
- `FgColorWhite` =   `ForegroundBlue  | ForegroundGreen | ForegroundRed`: Text color is white.
---
- `FgColorBlackBright` =   `FgColorBlack   | ForegroundIntensity`: Text color is bright black.
- `FgColorBlueBright` =    `FgColorBlue    | ForegroundIntensity`: Text color is bright blue.
- `FgColorGreenBright` =   `FgColorGreen   | ForegroundIntensity`: Text color is bright green.
- `FgColorCyanBright` =    `FgColorCyan    | ForegroundIntensity`: Text color is bright cyan.
- `FgColorRedBright` =     `FgColorRed     | ForegroundIntensity`: Text color is bright red.
- `FgColorMagentaBright` = `FgColorMagenta | ForegroundIntensity`: Text color is bright magenta.
- `FgColorYellowBright` =  `FgColorYellow  | ForegroundIntensity`: Text color is bright yellow.
- `FgColorWhiteBright` =   `FgColorWhite   | ForegroundIntensity`: Text color is bright white.
---
- `BgColorBlack` =   `0`:                                                 Background color is black.
- `BgColorBlue` =    `BackgroundBlue`:                                    Background color is blue.
- `BgColorGreen` =   `BackgroundGreen`:                                   Background color is green.
- `BgColorCyan` =    `BackgroundBlue  | BackgroundGreen`:                 Background color is cyan.
- `BgColorRed` =     `BackgroundRed`:                                     Background color is red.
- `BgColorMagenta` = `BackgroundBlue  | BackgroundRed`:                   Background color is magenta.
- `BgColorYellow` =  `BackgroundGreen | BackgroundRed`:                   Background color is yellow.
- `BgColorWhite` =   `BackgroundBlue  | BackgroundGreen | BackgroundRed`: Background color is white.
---
- `BgColorBlackBright` =   `BgColorBlack   | BackgroundIntensity`: Background color is bright black.
- `BgColorBlueBright` =    `BgColorBlue    | BackgroundIntensity`: Background color is bright blue.
- `BgColorGreenBright` =   `BgColorGreen   | BackgroundIntensity`: Background color is bright green.
- `BgColorCyanBright` =    `BgColorCyan    | BackgroundIntensity`: Background color is bright cyan.
- `BgColorRedBright` =     `BgColorRed     | BackgroundIntensity`: Background color is bright red.
- `BgColorMagentaBright` = `BgColorMagenta | BackgroundIntensity`: Background color is bright magenta.
- `BgColorYellowBright` =  `BgColorYellow  | BackgroundIntensity`: Background color is bright yellow.
- `BgColorWhiteBright` =   `BgColorWhite   | BackgroundIntensity`: Background color is bright white.

Following operators are overloaded for this enumeration:

- `constexpr Attribute operator|(Attribute a, Attribute b);`
- `constexpr Attribute operator&(Attribute a, Attribute b);`
- `constexpr Attribute operator^(Attribute a, Attribute b);`

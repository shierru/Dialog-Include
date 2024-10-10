# Dialog-Include
[![MPL-2.0 License](https://img.shields.io/badge/License-MPL2.0-green.svg)](https://www.mozilla.org/en-US/MPL/2.0)

> This repository contains the code for the Dialog library.

## Installation
Simple installation method:
```
sampctl package install Dialog-Include
```

You can then incorporate it into your game mode/script:
```
#include <Dialog-Include\dialog>
```

## Functions
#### Syntax


```pawn
Dialog_Show(playerid, func[])
```


#### Parameters


| 	**Name**	 | 	**Info**	 |
|	---	|	---	|
| 	`playerid`	 | 	player ID	 |
| 	`func`	 | 	The name of the dialog to show to the player	 |

#### Returns
Always returns true.

#### Remarks
Shows the player a pre-created template for a dialog (`DialogCreate:`).

#### Syntax


```pawn
Dialog_Open(playerid, func[], DIALOG_STYLE:style, caption[], info[], button1[], button2[])
```


#### Parameters


| 	**Name**	 | 	**Info**	 |
|	---	|	---	|
| 	`playerid`	 | 	player ID	 |
| 	`func`	 | 	The name of the dialog to show to the player	 |
| 	`style`	 | 	The style of the dialog	 |
| 	`title`	 | 	The title at the top of the dialog	 |
| 	`body`	 | 	The text to display in the main dialog	 |
| 	`button1`	 | 	The text on the left button	 |
| 	`button2`	 | 	The text on the right button	 |

#### Returns
Always returns the execution value of the `ShowPlayerDialog` function.

#### Remarks
Shows dialog to the player.

#### Syntax


```pawn
Dialog_Close(playerid)
```


#### Parameters


| 	**Name**	 | 	**Info**	 |
|	---	|	---	|
| 	`playerid`	 | 	player ID	 |

#### Returns
Depending on the presence of the `HidePlayerDialog` function, it returns either the result of executing the first function, or the result of `ShowPlayerDialog`.

#### Remarks
Closes the dialog shown to the player in case the player has it open.

#### Syntax


```pawn
Dialog_IsOpen(playerid, func[] = "")
```


#### Parameters


| 	**Name**	 | 	**Info**	 |
|	---	|	---	|
| 	`playerid`	 | 	player ID	 |
| 	`func`	 | 	The name of dialog	 |

#### Remarks
Checks if the player has any dialog open, or (depending on the optional parameter) checks if the player has any dialog open.

## Contributing

Pull requests are welcome. 

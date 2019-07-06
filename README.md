# Self finding chronicles

TODO: write introduction

## Generate and render the data folder

If the game get launched on full HD (16:9), one grid size unit is 40 px and the screen has 32x18 grid units.
For drawing the krita assets, one grid size unit (40x40 px) will be maped to 500x500 px. So the smalest canvas 
for the game, i.e. a plant or an item is 500x500 px. 
The `generate-data` script will render png data out of the krita files and resizes the png file to the apropriate 
grid unit size for performance and filesize reasons.    

First step is generating a equal sized png file out of the krita drawing.

    krita ${KRITA_FILE} --export --export-filename ${PNG_FILE}

Then the result png will be resized to he specified grid unit size and stored in the data folder.

    


## Create sprite out of single images

    $ cd data/images/characters/character-run
    $ ls
      0.png  1.png  2.png  3.png 
    $ convert *.png +append sprite.png

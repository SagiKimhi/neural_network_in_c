# NNC

A small, simple neural networks training framework written in C.

## About

NNC is a small and simple neural networks framework written in C.

I have originally implemented this library as a side project to educate myself.

The original inspiration for the project came from tsoding's deep learning 
series (see: [tsoding/nn.h](https://github.com/tsoding/nn.h)).

Although it is based on the series, I did my best to keep the implementation
to my own coding style and logical thought process as this is meant to be an 
educational project and theres no better leaning expreience than dealing with 
problems only a dumbass such as myself could ever create for myself.

In other words, this is an ok implementation from a wannabe good C programmer,
of a better project originally authored by tsoding, and so if this library is
somehow useful to you then I encourage you to first check out [tsoding/nn.h](https://github.com/tsoding/nn.h)
as his implementation is most probably multiple times better then mine and is
accompanied with a respectful amount of additional educational contents.

---

## Training with GUI

### Model training with GUI
<img src="assets/training.gif" height="600" width="850">

### Real-time training output visualization 
<img src="assets/transitions.gif" height="600" width="850">

### Exporting output as image, video, or user defined method
<img src="assets/upscale.gif" height="600" width="850">

---

## Building and Running

### Dependencies

To compile and run the GUI examples, raylib must be installed and linked with
the binary.

Installation of raylib is fairly simple, and might even be provided by your
package manager (i.e. apt, brew, etc.), for additional information and
detailed build/installation instructions, visit [raysan5/raylib.git](https://github.com/raysan5/raylib.git)

### Cloning a local copy

clone repo
```sh
git clone git@github.com:SagiKimhi/neural_networks_in_c.git
```

cd into local repo clone
```sh
cd neural_networks_in_c
```

### Building, running, and cleaning

Build all examples
```sh
make # builds all examples, can be cleaned later with make clean
```

Alternatively, clean previous build by running `make clean`, and start a new 
build of a single example instead of building everything.
```sh
make clean \ # Clean previous build
&& make img_upscale_gui \ # Build image upscale GUI example
&& ./nn_img_upscale_gui # Run the image upscale GUI example
```

after running the example, you should be prompted to provide a path to a
directory with images to train the model on: 
```sh
Please enter the path to your desired training images directory:
```

you can provide the path to the "training_data" directory which contains a few 
images from the MNIST dataset for the purpose if this example:
```sh
  Please enter the path to ... : ./training_data
```

---

### Runtime CLI help menu (H Key)

Upon hitting Enter the GUI will open up, the following key mappings are
available to start, pause, restart, and control the live training process:

- `h`/`H` - Print help menu to console (stdout).
- `r`/`R` - Restart training and re-initialize all parameters to random values
- `Spacebar` - Start/Stop the training process.
- `Arrow Up` - Increse learning rate by 0.1
- `Arrow Down` - Decrease learning rate by 0.1
- `Shift + Arrow Up` - Increse learning rate by 0.001
- `Shift + Arrow Down` - Decrease learning rate by 0.001

Additional keybinds and actions are available in the example's help menu (hit
the `h` key and inspect the options printed to console), these include
operations for saving/loading the model, exporting upscaled image outputs from
model to png, or exporting a video of the outputs and transitions as video via
ffmpeg (must be installed and available under $PATH environment variable).

### Example/Reference:

[build example GIF](assets/building.gif)

---

## Contributions

If you ever find this useful to you even after visiting tsoding's
implementation, then please do feel free to use/fork/contribute to this
project. 

I am usually quite active on github and will be glad to review any pull 
requests and/or issues if it is helpful or useful to anyone in any way.

Theres no formal flow or laws for contributions (there aren't any contributions
as well kekw).

As a recommendation, I can suggest the following flow which I find useful on
my day to day projects both as an individual as well as when maintaining
and/or contributing to team projects at work:

1. Fork and clone the repository.

2. Create a new issue for whatever it is you will be working on, or select an
   existing issue from the [issues](issues/) page.

3. Assign the issue to yourself, and create a dedicated branch for working on
   that issue using one of the name patterns listed below:

    - fix/\<issue-id\>/\<author\> : Use for fixing issues labled as 'bug'.

    - doc/\<issue-id\>/\<author\> : Use for issues labled as 'documentation'.

    - feat/\<issue-id\>/\<author\> : Use for issues labled as 'enhancement'.

    - maint/\<issue-id\>/\<author\> : Use for issues labled as 'maintnance'.

    - refactor/\<issue-id\>/\<author\> : Use for issues labled as 'refactor'.

4. Commit your changes and create a new pull request describing the change.

If the request is not merged after a couple of days, and is not closed nor
received any attention by me, then I probably haven't noticed it yet, in
such case do feel free to email me at sagi.kim5@gmail.com, or even send me 
a dm on linkedin, any short piece of text notifying me about the pr should do 
the trick, e.g:

   > Hey, i have added the following pr - <link-to-pr>, could you have a look
   > please?

## License (to keep lawyers happy)

MIT License

Copyright (c) 2025 Sagi Kimhi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


## Additional Resources

### Fix commit and squash

```sh
# make changes for commit 1
git add <files>
git commit -s -m "MSG"


# make changes for commit 2
git add <files>
git commit -s -m "MSG"


# more changes for commit 1 in a third commit
git add <files>


# this will create a special commit message that git autosquash will
# automatically understand
#
# <rev> can be the sha256 hash of the commit, HEAD^, or even: "some text in 
# commit 1's commit message"
git commit --fixup=<rev>  


# open editor with the commits already rearranged and commit 1 + commit 3 ready
# to be squashed together
git rebase -i --autosquash 
```


### Useful guidelines for submitting patches and commiting changes

[git/git.git - Submitting Patches](assets/SubmittingPatches.html):
General guidelines for submitting patches and commiting changes from the
original git repository of git (git.git)


### Git Cheatsheet

<img alt="Git Cheatsheet"
    src="assets/git-cheatsheet.png"
/>


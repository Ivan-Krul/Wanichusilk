[delay] 50
[act][null]
[task][get][task #1] // mark what it's hotkey
[if][task #1] |-> [goto] 15 // if it completed
[text][Calamus][smile] "Hello"
[text][Calamus][] "We with Alula are trying to get water moss"
[act][Alula turn]
[text][Alula][] "Yes, my bro wanted it!"
[text][Alula][speak] "He think, what there can get some phosphor"
[text][Niko][2] "Oh, can I help you with this"
[choose][Niko] "Of course!" |-> [text][Niko][smile] "Of cource!" |, "Not now" |-> [text][Alula][oh] "Oh c'mon!" |-> [act][null] |-> [end]
[text][Calamus][] "Of course! You can find some"
[text][Calamus][smile] "for example, basket, I want more"
[text][Alula][speak] "Tee-hee, he want a lot!"
[task][add][task #1]
[act][null] // reset
[end]
[text][Alula][] "Oh, hello, Niko!"
[text][Alula][gahp] "*gahp* It's so beauty!"
[act][Alula jump]
[text][Alula][gahp] "Where did you find it!?"
[text][Calamus][heh] "She is so excited, really, where did you find it?"
[text][Niko][83c] "Oh, I have traded in with Magpie"
[text][Calamus][shock] "Hold on! On What has you traded!?"
[choose][Niko] "Instead of the Light-bulb" |-> [next] |, "Instead of a notebook" |-> [goto] 31
[delay] 250
[text][Niko][distressed2] "Instead of the Light-bulb"
[text][Calamus][shock] "Whaaaaaat!!?" |-> [text][Alula][oh] "What?" 
[text][Niko][distressed_talk] "I..." |-> [text][Niko][distressed_meow] "has lost it..." |->  [text][Niko][cry] ""
[text][action][] "Darkness is filled everywhere..."
[text][action][messagebox] "You failed with rescue of Niko!"
[text][action][messagebox] "And now, enjoy your darkness"
[wait] 5000
[end]
[file][monolang.das] 1
[task][done][task #1]
[end]
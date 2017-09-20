<h3 class="modal-title" id="myModalLabel">PacMan</h3>
        <video height="100%" controls>
            <source src="assets/PacMan.mp4" type="video/mp4">
            Your browser does not support the video tag.
        </video>
        <h4>Project Overview</h4>
        <p>For my final project of my first computing class, I programmed the retro arcade game Pac-man in C. I made a few simplifications to the original game, but it plays very similarly to the original. First, I used a grid of 17x15, upon which the board was drawn. Each box inside the grid corresponds to 40 square pixels, which both Pac-Man and all of the ghosts can fit inside entirely. Also, due to this smaller nature of the board, the number of pellets was reduced to 155 instead of the classic 240 dots. Due to the primitive graphics engine I used (I had to hand code each element on the board as a combination of lines), the game does not have continuous motion. The user has the ability to move to the next loop by pressing any button. This allows for less rendering time and a more enjoyable user experience.</p>
        <h4>Specifics of the Game</h4>
        <p>The user is able to control Pac-Man's movement with the use of the arrow keys.
        When the game is launched, the title screen appears, which introduces the
        ghosts and point values for the pellets. It also has the instructions of how
        to play the game. Once a button is pressed, the user controls Pac-Man's
        movements with the arrow keys. Every time any key is pressed, Pac-Man and the
        ghosts move accordingly. Here are the features of the game:</p>
        <ul>
            <li>The red ghost, "Blinky," starts outside of the Ghost
            House, and immediately begins targeting Pac-Man. After 10 movements, Pinky
            comes out, and then after another 20 and 40 moves respectively, Inky and Clyde 
            leave the Ghost House. </li>
            <li>Whenever a Power Pellet is eaten, the ghosts all enter "frightened" mode, 
            turn blue, and can be eaten by Pac-Man. During this "frightened" state, the 
            ghosts all slow down to half of their orignial speed, thus allowing Pac-Man 
            to catch and eat them easier. When they are eaten, the number of points is 
            shown on the screen, right on top of Pac-Man. Each ghost is worth 200 points, 
            but if more than one are eaten during a single "frightened" mode, then the 
            number is 2^(n)*200 points per ghost. When they are eaten, they become eyes 
            and venture back to the Ghost House to respawn. Once the eyes go into the 
            Ghost House, they regain their body and they immediately leave the house 
            onto the board. Lastly, when the frightened state is almost over, the 
            remaining frightened ghosts flash blue and white to show they are about to
            enter the normal mode again.</li>
            <li>When Pac-Man touches a ghost that is not frightened or dead, he dies. This
            death is shown by a series of drawings. The user is instructed to click a few
            times until the animation is over. I chose to do this instead of the animation
            because actual animation caused the rendering to lag tremendously.
            The death animation concludes with Pac-Man
            losing a life, shown to the left of the board. If this was Pac-Man's last
            life, the middle of the board displays GAME OVER, and asks the user if he/she
            would like to play again.</li>
            <li>On the left of the board is a score counter. While it does not store scores
            between games, it still displays the score as the original Pac-Man games did.
            Each pellet that Pac-Man eats is 10 points, except for the power pellets (the
            big dots), which are worth 50 each. Also, eating ghosts is worth points, as
            described above. The score is added up and displayed after every movement.</li>
        </ul>
<p>I wanted to elaborate more on the Ghost's movements (because I am very proud of this project), so I did so below.</p>

<h3 class="modal-title" id="myModalLabel">PacMan's Ghost's Movement Algorithms</h3>
        <video height="500" controls>
            <source src="assets/PacMan.mp4" type="video/mp4">
            Your browser does not support the video tag.
        </video>
        <p>The most complicated part of the game was programming the ghosts. Each ghost
        had three (but actually five) different states. These were: chase, scatter,
        frighten, dead, and housed. Each of these states caused the ghosts to move
        differently.</p>
        <p>The ghosts movement followed the actual movement in the original Pac-Man game.
        Every time a ghost moved, it did a few things. First, it selected a target,
        which is selected based on its state and targeting mechanism.
        Each ghost has a distinct targeting mechanism, and these targets were selected
        in the targetGhosts function. When in the chase state, the ghosts tried to get
        to Pac-Man, but each in its own way. </p>
        <ul>
            <li>Blinky, the red ghost, targets pacman directly. His target is pacman's current 
            position.</li>
            <li>Pinky, the pink ghost, takes Pac-Man's orientation (up,down,left,or right)
            and targets three spaces in that direction from Pac-Man's location (in the
            original Pac-Man, it was 4, but due to the size of the board I made it 3)</li>
            <li>Inky, the blue ghost, uses both Pac-Man's location and Blinky's location. He
            takes the vector from Blinky's location to two spaces in front of Pac-Man's
            location, doubles it, and targets that space on the board.</li>
            <li>Clyde, the orange ghost, decides his target based on his proximity to
            Pac-Man. If he is greater than or equal to 5 (original game was 8) spaces from
            Pac-Man's location, he will target Pac-Man directly like Blinky. Otherwise, he
            will move toward his designated corner.</li>
        </ul> 
        <p>Each ghost has a designated corner that they will try to move toward during
        their scatter state. This scatter state allows for Pac-Man to not always be
        targeted by the ghosts, thus making the game more fun, easier, and more
        realistic. During this state, the ghosts all target their corner:</p>
        <table class="table table-striped table-bordered">
            <thead>
                    <tr>
                        <th>Ghost Name</th>
                        <th>Color</th>
                        <th>Target</th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td>Blinky</td>
                        <td>Red</td>
                        <td>Top Right</td>
                    </tr>
                    <tr>
                        <td>Pinky</td>
                        <td>Pink</td>
                        <td>Top Left</td>
                    </tr>
                    <tr>
                        <td>Inky</td>
                        <td>Blue</td>
                        <td>Bottom Right</td>
                    </tr>
                    <tr>
                        <td>Clyde</td>
                        <td>Orange</td>
                        <td>Bottom Left</td>
                    </tr>
            </tbody>
        </table>
        <p>Once a ghost has selected its target, it moves based on where it can and also
        which movment gets it closer to its target. A ghost can only move to spaces
        that are not 1 or 8 on the board (boundraies and the ghost house). Also,
        ghosts cannot move to where they just came from. If they go into an
        intersection, they can move either way, but cannot move back to their previous
        location. The locations of all the ghosts was stored in the array of struct
        Location. The struct Location included the current x and y, the previous x and
        y, and the orientation of the object included (Pac-Man also had his own
        Location struct for his location, along with the number 2 being placed on the 
        board array).</p>
        <p>To check whether Pac-Man or a ghost has died, the checkDeath function compares
        the locations of Pac-Man and all the ghosts. If they share a space, then one
        dies: the ghost if he is in frightened mode or Pac-Man otherwise. If a ghost
        dies, his target is then the top of the ghost house, he is drawn without a
        body, just eyes, and the number of points appears on Pac-Man's body. When a
        dead ghost gets to his target, it is "housed" and then goes into the house and
        immediately leaves.</p>
    </div>

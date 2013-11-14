'''
char_gen.py

The basic character generation module. Allows accounts to create new characters
with basic selection for name, sex, and race.
'''
import mudsys, mud, socket, char, hooks

def check_char_name(arg):
    '''checks to make sure the character name is valid. Names are valid if they
       are greater than 2 characters, less than 13, and comprise only alpha
       characters.'''
    return len(arg) >= 3 and len(arg) <= 12 and arg.isalpha()

def cg_name_handler(sock, arg):
    if not check_char_name(arg):
        sock.send("{cIllegal name, please pick another.{n\r\n")
    elif mudsys.player_exists(arg):
        sock.send("{cA player with that name already exists.{n\r\n")
    elif mudsys.player_creating(arg):
        sock.send("{cA player is already being created with that name.{n\r\n")
    elif arg.lower().startswith("guest"):
        sock.send("{cCharacter names cannot begin with 'guest'.{n\r\n")
    else:
        name = arg[0].upper() + arg[1:]
        ch = mudsys.create_player(name)

        if ch == None:
            sock.send("{cIllegal name, please pick another.{n\r\n")
        else:
            mudsys.attach_char_socket(ch, sock)
            ch.rdesc = ch.name + " is here."
            ch.desc = "You see a [me.height] [me.sex].  [me.heshe] has \
                [me.hair_length] [me.hair_texture] [me.hair_color] hair. \
                [me.hisher] [me.nose] nose is set over [me.lips] lips \
                and a [me.chin] chin in [me.hisher] [me.face] face."
            sock.pop_ih()

def cg_sex_handler(sock, arg):
    try:
        result = {
            'M' : 'male',
            'F' : 'female',
            }[arg[0].upper()]
        sock.ch.sex = result
        sock.pop_ih()
    except:
        sock.send("{cInvalid sex, try again.{n\r\n")

def cg_race_handler(sock, arg):
    if not mud.is_race(arg, True):
        sock.send("{cInvalid race selection, try again.{n\r\n")
    else:
        sock.ch.race = arg.lower()
        sock.pop_ih()

def cg_body_handler(sock, arg):
    body_types = {
        '1': 'athletic',
        '2': 'average',
        '3': 'bony',
        '4': 'burly',
        '5': 'chubby',
        '6': 'curvaceous',
        '7': 'heavy',
        '8': 'large',
        '9': 'lean',
        '10': 'muscular',
        '11': 'rugged',
        '12': 'skinny',
        '13': 'slender' }
    sock.ch.body_type = body_types.get(arg, 2)
    sock.pop_ih()

def cg_complexion_handler(sock, arg):
    complexions = {
        '1':  'pale',
        '2':  'fair',
        '3':  'common',
        '4':  'lightly tanned',
        '5':  'ocher',
        '6':  'sandalwood',
        '7':  'light brown',
        '8':  'brown',
        '9':  'dark',
        '10': 'swarthy',
        '12': 'black',
        '13': 'tanned' }
    sock.ch.complexion = complexions.get(arg, 3)
    sock.pop_ih()

def cg_eye_color_handler(sock, arg):
    eye_colors = {
        '1':  'obsidian',
        '2':  'ebony',
        '3':  'slate',
        '4':  'dark grey',
        '5':  'grey',
        '6':  'light grey',
        '7':  'dark brown',
        '8':  'brown',
        '9':  'hazel',
        '10': 'jade',
        '11': 'green',
        '12': 'sea green',
        '13': 'light blue',
        '14': 'azure',
        '15': 'blue',
        '16': 'dark blue' }
    sock.ch.eye_color = eye_colors.get(arg, 'brown')
    sock.pop_ih()

def cg_hair_color_handler(sock, arg):
    hair_colors = {
        '1':  'bald',
        '2':  'black',
        '3':  'midnight black',
        '4':  'raven',
        '5':  'dark grey',
        '6':  'grey',
        '7':  'brown',
        '8':  'walnut',
        '9':  'chestnut',
        '10': 'ash brown',
        '11': 'dark brown',
        '12': 'light brown',
        '13': 'auburn',
        '14': 'light blonde',
        '15': 'blonde',
        '16': 'strawberry blonde',
        '17': 'red',
        '18': 'dark red',
        '19': 'white',
        '20': 'ash' }
    sock.ch.hair_color = hair_colors.get(arg, 2)
    sock.pop_ih()

def cg_hair_length_handler(sock, arg):
    hair_lengths = {
        '1':  'short',
        '2':  'close-cropped',
        '3':  'shoulder-length',
        '4':  'long',
        '5':  'waist-length' }
    sock.ch.hair_length = hair_lengths.get(arg, 3).lower()
    sock.pop_ih()

def cg_hair_texture_handler(sock, arg):
    hair_textures = {
        '1':  'billowy',
        '2':  'bouncy',
        '3':  'bristly',
        '4':  'bushy',
        '5':  'clean',
        '6':  'coarse',
        '7':  'curly',
        '8':  'delicate',
        '9':  'feathery',
        '10': 'fine',
        '11': 'flowing',
        '12': 'glistening',
        '13': 'glossy',
        '14': 'greasy',
        '15': 'kinky',
        '16': 'lively',
        '17': 'lush',
        '18': 'lustrous',
        '19': 'shaggy',
        '20': 'spiked',
        '21': 'soft',
        '22': 'straight',
        '23': 'sweeping',
        '24': 'thick',
        '25': 'warvy',
        '26': 'wispy' }
    sock.ch.hair_texture = hair_textures.get(arg, 5)
    sock.pop_ih()

def cg_face_handler(sock, arg):
    faces = {
        '1':  'gentle',
        '2':  'freckled',
        '3':  'long',
        '4':  'ordinary',
        '5':  'pleasant',
        '6':  'plain',
        '7':  'scarred',
        '8':  'round',
        '9':  'narrow',
        '10': 'disfigured' }
    sock.ch.face = faces.get(arg, 4)
    sock.pop_ih()

def cg_nose_handler(sock, arg):
    noses = {
        '1':  'large',
        '2':  'wide',
        '3':  'hooked',
        '4':  'pointed',
        '5':  'thin',
        '6':  'crooked',
        '7':  'prominent',
        '8':  'button',
        '9':  'ordinary',
        '10': 'plain' }
    sock.ch.nose = noses.get(arg, 9)
    sock.pop_ih()

def cg_lips_handler(sock, arg):
    lips = {
        '1':  'full',
        '2':  'thin',
        '3':  'pouting',
        '4':  'sneering',
        '5':  'pursed',
        '6':  'round',
        '7':  'small',
        '8':  'curved' }
    sock.ch.lips = lips.get(arg, 2)
    sock.pop_ih()

def cg_chin_handler(sock, arg):
    chins = {
        '1':  'cleft',
        '2':  'double',
        '3':  'plain',
        '4':  'pointed',
        '5':  'rugged',
        '6':  'square',
        '7':  'ordinary',
        '8':  'round' }
    sock.ch.chin = chins.get(arg, 7)
    sock.pop_ih()

def cg_height_handler(sock, arg):
    heights = {
        '1':  'very short',
        '2':  'short',
        '3':  'average',
        '4':  'tall',
        '5':  'very tall' }
    sock.ch.height = heights.get(arg, 3)
    sock.pop_ih()

def cg_finish_handler(sock, arg):
    # pop our input handler for finishing character generation
    sock.pop_ih()

    # log that the character created
    mud.log_string("New player: " + sock.ch.name + " has entered the game.")
    
    # register and save him to disk and to an account
    mudsys.do_register(sock.ch)

    # make him exist in the game for functions to look him up
    mudsys.try_enter_game(sock.ch)

    # run the init_player hook
    hooks.run("init_player", hooks.build_info("ch", (sock.ch,)))
    
    # attach him to his account and save the account
    sock.account.add_char(sock.ch)
    mudsys.do_save(sock.account)
    mudsys.do_save(sock.ch)

    # clear their screen
    sock.ch.act("clear")
    
    # send them the motd
    sock.ch.page(mud.get_motd())

    # make him look at the room
    sock.ch.act("look")

    # run our enter hook
    hooks.run("enter", hooks.build_info("ch rm", (sock.ch, sock.ch.room)))

def cg_name_prompt(sock):
    sock.send_raw("What is your character's name? ")

def cg_sex_prompt(sock):
    sock.send_raw("What is your sex (M/F)? ")

def cg_race_prompt(sock):
    sock.send("Available races are: ")
    sock.send(mud.list_races(True))
    sock.send_raw("\r\nPlease enter your choice: ")

def cg_body_prompt(sock):
    sock.send_raw("What is your body type?")
    sock.send_raw("\n\r")
    sock.send_raw("  [1]  Athletic\n\r")
    sock.send_raw("  [2]  Average\n\r")
    sock.send_raw("  [3]  Bony\n\r")
    sock.send_raw("  [4]  Burly\n\r")
    sock.send_raw("  [5]  Chubby\n\r")
    sock.send_raw("  [6]  Curvaceous\n\r")
    sock.send_raw("  [7]  Heavy\n\r")
    sock.send_raw("  [8]  Large\n\r")
    sock.send_raw("  [9]  Lean\n\r")
    sock.send_raw("  [10] Muscular\n\r")
    sock.send_raw("  [11] Rugged\n\r")
    sock.send_raw("  [12] Skinny\n\r")
    sock.send_raw("  [13] Slender\n\r")

def cg_complexion_prompt(sock):
    sock.send_raw("What is your complexion?")
    sock.send_raw("\n\r")
    sock.send_raw("  [1]  Pale\n\r")
    sock.send_raw("  [2]  Fair\n\r")
    sock.send_raw("  [3]  Common\n\r")
    sock.send_raw("  [4]  Tanned\n\r")
    sock.send_raw("  [5]  Ocher\n\r")
    sock.send_raw("  [6]  Sandalwood\n\r")
    sock.send_raw("  [7]  Light brown\n\r")
    sock.send_raw("  [8]  Brown\n\r")
    sock.send_raw("  [9]  Dark\n\r")
    sock.send_raw("  [10] Swarthy\n\r")
    sock.send_raw("  [11] Black\n\r")
    sock.send_raw("  [12] Lightly tanned\n\r")

def cg_eye_color_prompt(sock):
    sock.send_raw("What color are your eyes?")
    sock.send_raw("\n\r")
    sock.send_raw("  [1]  Obsidian\n\r")
    sock.send_raw("  [2]  Ebony\n\r")
    sock.send_raw("  [3]  Slate\n\r")
    sock.send_raw("  [4]  Dark grey\n\r")
    sock.send_raw("  [5]  Grey\n\r")
    sock.send_raw("  [6]  Light grey\n\r")
    sock.send_raw("  [7]  Dark brown\n\r")
    sock.send_raw("  [8]  Brown\n\r")
    sock.send_raw("  [9]  Hazel\n\r")
    sock.send_raw("  [10] Jade\n\r")
    sock.send_raw("  [11] Green\n\r")
    sock.send_raw("  [12] Sea green\n\r")
    sock.send_raw("  [13] Light blue\n\r")
    sock.send_raw("  [14] Azure\n\r")
    sock.send_raw("  [15] Blue\n\r")
    sock.send_raw("  [16] Dark blue\n\r")

def cg_hair_color_prompt(sock):
    sock.send_raw("What color is your hair?\n\r")
    sock.send_raw("\n\r")
    sock.send_raw("  [1]  Bald\n\r")
    sock.send_raw("  [2]  Black\n\r")
    sock.send_raw("  [3]  Midnight black\n\r")
    sock.send_raw("  [4]  Raven\n\r")
    sock.send_raw("  [5]  Dark grey\n\r")
    sock.send_raw("  [6]  Grey\n\r")
    sock.send_raw("  [7]  Brown\n\r")
    sock.send_raw("  [8]  Walnut\n\r")
    sock.send_raw("  [9]  Chestnut\n\r")
    sock.send_raw("  [10] Ash brown\n\r")
    sock.send_raw("  [11] Dark brown\n\r")
    sock.send_raw("  [12] Light brown\n\r")
    sock.send_raw("  [13] Auburn\n\r")
    sock.send_raw("  [14] Light blonde\n\r")
    sock.send_raw("  [15] Blonde\n\r")
    sock.send_raw("  [16] Strawberry blonde\n\r")
    sock.send_raw("  [17] Red\n\r")
    sock.send_raw("  [18] Dark red\n\r")
    sock.send_raw("  [19] White\n\r")
    sock.send_raw("  [20] Ash\n\r")

def cg_hair_length_prompt(sock):
    sock.send_raw("How long is your hair?")
    sock.send_raw("\n\r")
    sock.send_raw("  [1]  Short\n\r")
    sock.send_raw("  [2]  Close-cropped\n\r")
    sock.send_raw("  [3]  Shoulder-length\n\r")
    sock.send_raw("  [4]  Long\n\r")
    sock.send_raw("  [5]  Waist-length\n\r")

def cg_hair_texture_prompt(sock):
    sock.send_raw("What is the texture of your hair?")
    sock.send_raw("\n\r")
    sock.send_raw("  [1]  Billowy\n\r")
    sock.send_raw("  [2]  Bouncy\n\r")
    sock.send_raw("  [3]  Bristly\n\r")
    sock.send_raw("  [4]  Bushy\n\r")
    sock.send_raw("  [5]  Clean\n\r")
    sock.send_raw("  [6]  Coarse\n\r")
    sock.send_raw("  [7]  Curly\n\r")
    sock.send_raw("  [8]  Delicate\n\r")
    sock.send_raw("  [9]  Feathery\n\r")
    sock.send_raw("  [10] Fine\n\r")
    sock.send_raw("  [11] Flowing\n\r")
    sock.send_raw("  [12] Glistening\n\r")
    sock.send_raw("  [13] Glossy\n\r")
    sock.send_raw("  [14] Greasy\n\r")
    sock.send_raw("  [15] Kinky\n\r")
    sock.send_raw("  [16] Lively\n\r")
    sock.send_raw("  [17] Lush\n\r")
    sock.send_raw("  [18] Lustrous\n\r")
    sock.send_raw("  [19] Shaggy\n\r")
    sock.send_raw("  [20] Spiked\n\r")
    sock.send_raw("  [21] Soft\n\r")
    sock.send_raw("  [22] Straight\n\r")
    sock.send_raw("  [23] Sweeping\n\r")
    sock.send_raw("  [24] Thick\n\r")
    sock.send_raw("  [25] Wavy\n\r")
    sock.send_raw("  [26] Wispy\n\r")

def cg_face_prompt(sock):
    sock.send_raw("What does your face look like?")
    sock.send_raw("\n\r")
    sock.send_raw("  [1]  Gentle\n\r")
    sock.send_raw("  [2]  Freckled\n\r")
    sock.send_raw("  [3]  Long\n\r")
    sock.send_raw("  [4]  Ordinary\n\r")
    sock.send_raw("  [5]  Narrow\n\r")
    sock.send_raw("  [6]  Pleasant\n\r")
    sock.send_raw("  [7]  Plain\n\r")
    sock.send_raw("  [8]  Scarred\n\r")
    sock.send_raw("  [9]  Round\n\r")
    sock.send_raw("  [10] Disfigured\n\r")

def cg_nose_prompt(sock):
    sock.send_raw("What is the shape of your nose?")
    sock.send_raw("\n\r")
    sock.send_raw("  [1]  Large\n\r")
    sock.send_raw("  [2]  Wide\n\r")
    sock.send_raw("  [3]  Hooked\n\r")
    sock.send_raw("  [4]  Pointed\n\r")
    sock.send_raw("  [5]  Thin\n\r")
    sock.send_raw("  [6]  Crooked\n\r")
    sock.send_raw("  [7]  Prominent\n\r")
    sock.send_raw("  [8]  Button\n\r")
    sock.send_raw("  [9]  Ordinary\n\r")
    sock.send_raw("  [10] Plain\n\r")

def cg_lips_prompt(sock):
    sock.send_raw("What is the shape of your lips")
    sock.send_raw("\n\r")
    sock.send_raw("  [1]  Full\n\r")
    sock.send_raw("  [2]  Thin\n\r")
    sock.send_raw("  [3]  Pouting\n\r")
    sock.send_raw("  [4]  Sneering\n\r")
    sock.send_raw("  [5]  Pursed\n\r")
    sock.send_raw("  [6]  Round\n\r")
    sock.send_raw("  [7]  Small\n\r")
    sock.send_raw("  [8]  Curved\n\r")

def cg_chin_prompt(sock):
    sock.send_raw("What does your chin look like?")
    sock.send_raw("\n\r")
    sock.send_raw("  [1]  Cleft\n\r")
    sock.send_raw("  [2]  Double\n\r")
    sock.send_raw("  [3]  Plain\n\r")
    sock.send_raw("  [4]  Pointed\n\r")
    sock.send_raw("  [5]  Rugged\n\r")
    sock.send_raw("  [6]  Square\n\r")
    sock.send_raw("  [7]  Ordinary\n\r")
    sock.send_raw("  [8]  Round\n\r")

def cg_height_prompt(sock):
    sock.send_raw("How tall are you?")
    sock.send_raw("\n\r")
    sock.send_raw("  [1]  Very short\n\r")
    sock.send_raw("  [2]  Short\n\r")
    sock.send_raw("  [3]  Average\n\r")
    sock.send_raw("  [4]  Tall\n\r")
    sock.send_raw("  [5]  Very tall\n\r")

def cg_finish_prompt(sock):
    sock.send_raw("{c*** Press enter to finish character generation:{n ")



################################################################################
# character generation hooks
################################################################################
def char_gen_hook(info):
    '''Put a socket into the character generation menu when character generation
       hooks are called.
    '''
    sock, = hooks.parse_info(info)
    sock.push_ih(mudsys.handle_cmd_input, mudsys.show_prompt, "playing")
    sock.push_ih(cg_finish_handler, cg_finish_prompt)
    sock.push_ih(cg_height_handler, cg_height_prompt)
    sock.push_ih(cg_chin_handler, cg_chin_prompt)
    sock.push_ih(cg_lips_handler, cg_lips_prompt)
    sock.push_ih(cg_nose_handler, cg_nose_prompt)
    sock.push_ih(cg_face_handler, cg_face_prompt)
    sock.push_ih(cg_hair_texture_handler, cg_hair_texture_prompt)
    sock.push_ih(cg_hair_length_handler, cg_hair_length_prompt)
    sock.push_ih(cg_hair_color_handler, cg_hair_color_prompt)
    sock.push_ih(cg_eye_color_handler, cg_eye_color_prompt)
    sock.push_ih(cg_complexion_handler, cg_complexion_prompt)
    sock.push_ih(cg_body_handler, cg_body_prompt)
    sock.push_ih(cg_race_handler, cg_race_prompt)
    sock.push_ih(cg_sex_handler, cg_sex_prompt)
    sock.push_ih(cg_name_handler, cg_name_prompt)

def guest_gen_hook(info):
    sock, = hooks.parse_info(info)
    sock.push_ih(mudsys.handle_cmd_input, mudsys.show_prompt, "playing")

    # make the guest character
    ch = mudsys.create_player("Guest")

    # oops...
    if ch == None:
        sock.send("Sorry, there were issues creating a guest account.")
        sock.close()

    mudsys.attach_char_socket(ch, sock)
    ch.rdesc = "a guest player is here, exploring the world."
    ch.name  = ch.name + str(ch.uid)
    ch.race  = "human"

    # log that the character created
    mud.log_string("Guest character created (id %d)." % ch.uid)

    # make him exist in the game for functions to look him up
    mudsys.try_enter_game(ch)

    # run the init_player hook
    hooks.run("init_player", hooks.build_info("ch", (ch,)))

    # clear our screen
    ch.act("clear")

    # send them the motd
    ch.page(mud.get_motd())
    
    # make him look at the room
    ch.act("look")

    # run our enter hook
    hooks.run("enter", hooks.build_info("ch rm", (ch, ch.room)))



################################################################################
# loading and unloading the module
################################################################################
hooks.add("create_character", char_gen_hook)
hooks.add("create_guest",     guest_gen_hook)

def __unload__():
    '''removes the hooks for character generation'''
    hooks.remove("create_character", char_gen_hook)
    hooks.remove("create_guest",     guest_gen_hook)


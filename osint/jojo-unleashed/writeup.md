## Jojo Unleashed 

### Description

When tracing Jojo's kidnappers, you stumbled upon an MP3 file. With cautious anticipation, you clicked play, and as the sound filled the room, a hauntingly familiar hold tone echoed through the speakers, sending shivers down your spine.

The flag is the date and place of Jojo's kidnappers final meeting.

**Author: Sto**

### Solution

We listen to the MP3 provided with the challenge. It is apparently a French song. We try identifying it (using Shazam for instance) and we find out that it is [**Jacques Brel's Song named Jojo**](https://www.youtube.com/watch?v=unw0ECCwMQU).

Also, we use exiftool on the recording to see its details and we see the following comment :
```
Our meeting will be in a castle at the east of the first location mentioned in the song and named after its full name. Be careful Js! See ya ;)
```

After examining the complete lyrics of the song, we notice that the first location mentioned is Saint Cast. A quick search on Google reveals that its full name is Saint-Cast-Le-Guildo. Additionally, we search for castles to the east of Saint Cast and come across Château du Guildo, which is likely the location referenced in the comment.

We checked reviews on Google Maps and TripAdvisor, where we stumbled upon a peculiar review authored by JustineJB123. It contains a mention of **Jojo**.

![justinejb1.png](justinejb1.png)

We check her profile and we find the following description :
```
I'm always on the lookout for exciting adventures. If you're planning a trip and wouldn't mind good company, just email me on jorabetagnyjustine@gmail.com !
```
And we've got her email!
We use GHunt to obtain more information about her.
```
$ ghunt email jorabetagnyjustine@gmail.com

     .d8888b.  888    888                   888    
    d88P  Y88b 888    888                   888    
    888    888 888    888                   888    
    888        8888888888 888  888 88888b.  888888 
    888  88888 888    888 888  888 888 "88b 888    
    888    888 888    888 888  888 888  888 888    
    Y88b  d88P 888    888 Y88b 888 888  888 Y88b.  
     "Y8888P88 888    888  "Y88888 888  888  "Y888 v2

             By: mxrch (🐦 @mxrchreborn)
       Support my work on GitHub Sponsors ! 💖
    

          > GHunt 2.1.3 (BlackHat Edition) <         

🥳 New version 2.2.0 (Wardriving Edition) is available !
🤗 Run 'pipx upgrade ghunt' to update.

[+] Stored session loaded !

[+] New token for chrome has been generated
[+] Authenticated !

🙋 Google Account data

[+] Custom profile picture !
=> https://lh3.googleusercontent.com/a-/ALV-UjXJu2pMVzktnNlxfxtoeuHVZuzbM3diw_IFiRHDYP9eiw13t5U

[-] Default cover picture

Last profile edit : 2024/05/06 09:37:36 (UTC)

Email : jorabetagnyjustine@gmail.com
Gaia ID : 111504222788998010890

User types :
- GOOGLE_USER (The user is a Google user.)

📞 Google Chat Extended Data

Entity Type : PERSON
Customer ID : Not found.

🌐 Google Plus Extended Data

Entreprise User : False

🎮 Play Games data

[+] New token for playgames has been generated

[-] No player profile found.

🗺 Maps data

Profile page : https://www.google.com/maps/contrib/111504222788998010890/reviews

[Statistics]
Reviews : 1

[Reviews]
[+] Average rating : 5/5

[-] No costs data.

Target's locations preferences :

🏨 Generic [1]
- Public beach (1)
                                                   
[+] Probable location (confidence => Very low) :
- Saint-Cast-le-Guildo, France

🗓 Calendar data

[+] Public Google Calendar found !

Calendar ID : jorabetagnyjustine@gmail.com
Calendar Timezone : Europe/Paris

[+] 3 events dumped ! Showing the last 3 ones...

╔═════════════════════════════════════════════╤═════════════════════╤══════════╗
║                    Name                     │   Datetime (UTC)    │ Duration ║
╟─────────────────────────────────────────────┼─────────────────────┼──────────╢
║                First Meeting                │ 2022/06/03 20:30:00 │  1 hour  ║
╟─────────────────────────────────────────────┼─────────────────────┼──────────╢
║        Second Meeting - Almost There        │ 2023/06/03 20:30:00 │  1 hour  ║
╟─────────────────────────────────────────────┼─────────────────────┼──────────╢
║ Final Meeting - Last Step - Jojo is coming  │ 2024/06/03 20:10:00 │  1 hour  ║
╚═════════════════════════════════════════════╧═════════════════════╧══════════╝

🗃 Download link :
=> https://calendar.google.com/calendar/ical/jorabetagnyjustine@gmail.com/public/basic.ics
```

WWe can view the three meetings in the Google Calendar. After downloading it, we proceed to check its details.

We initially discover that the **date of the final meeting is June 3rd, 2024**.

In the second meeting description, we notice the @trebogosse tag :
```
Meeting Outline : 
- Manifesto is finally published (gracias to the @trebogosse for maintaining our website !)
- More J members are needed (let's hire!)
- Our next move to conquer the world (mwahaha)
 ```

 Let's look this up online. 
 We can find one [user](https://github.com/trebogosse) with such a name on Github which has one [repository](https://github.com/trebogosse/trepogosse). We also have the link of the website in the README https://trebogosse.github.io/trepogosse/.

![assets/justinejb1.png](trbogosse.png)

 By visiting it, we do not obtain much information apart from the fact that this is the **new version**. Hence, there must be an old one.

We use Wayback Machine to try finding it and we see a [first snapshot captured on May 4th](https://web.archive.org/web/20240504181359/https://trebogosse.github.io/trepogosse/). 
It is it :)
Upon inspecting the manifest and the page's content, we uncover the name of the manifesto's author: `Janette Voibien`

We try searching this name on social media, and we find a [Facebook profile](https://www.facebook.com/profile.php?id=61559613950990) with it. Indeed, it's hers and her Bio confirms that :
```
Master of Mischief, with antics aglow,
In the realm of laughter, I'm Jojo's show.
```
Her recent post was one describing her flight with a picture.

![jonjon.png](jonjon.png)
We look up the picture using Google Image Search and we discover that it was taken in the north of Tunisia, so Tunis Carthage is the closest Airport to that.

We track the possible flights destination using [FlightAware](https://fr.flightaware.com/) knowing that it goes from Tunis to somewhere else, fight duration is about 1h30 and it was early in the morning and it was on Thursday May, 23rd, 2024.

We end up with 3 options :

Tunis - Lyon
Tunis - Toulouse
Tunis - Nice

Moreover, a hint was given in the post descripiton about the destination which is _Needle Point_. When we translate it to French, it becomes "_Pointe de l'aiguille_" and making a Google Search about this gives us a location which is **Théoule sur Mer**, which aligns with the _Tunis - Nice_ flight.

And that kids is how we rescued Jojo!

#### Flag 

`N0PS{03-06-2024_theoule-sur-mer}`

new fullpage("#fullpage", {
  // mengaktifkan autoscroll dan tombol navigasi halaman
  autoScrolling: true,
  navigation: true,

  onLeave: (origin, destination, direction) => {
    // membuat konstanta untuk timeline
    const section = destination.item;
    console.log(destination.item);
    const title = section.querySelector("h1");
    const Subtitle = document.querySelector(".subtitle");
    const tim = document.querySelector(".team");
    const Tombol = document.querySelector(".gate");
    // create timeline
    const tl = new TimelineMax({ delay: 0.5 });
    // timeline section 1
    tl.fromTo(title, 0.5, { y: "50", opacity: 0 }, { y: 0, opacity: 1 });
    tl.fromTo(Subtitle, 0.5, { y: "50", opacity: 0 }, { y: 0, opacity: 1 });
    tl.fromTo(tim, 0.5, { x: "50", opacity: 0 }, { x: 0, opacity: 1 });
    // timeline section 3
    tl.fromTo(Tombol, 0.5, { y: "50", opacity: 0 }, { y: 0, opacity: 1 });

    if (destination.index === 1) {
      // create constanta for slide 2
      const description = document.querySelector(".description");
      const icon = document.querySelectorAll(".ico");
      // section 2 attribute
      tl.fromTo(description, 0.7, { y: "50", opacity: 0 }, { y: 0, opacity: 1 })
        .fromTo(icon, 0.7, { x: "175%" }, { x: "57%" })
        .fromTo(icon[0], 1, { opacity: 1 }, { opacity: 1 })
        .fromTo(icon[1], 1, { opacity: 0 }, { opacity: 1 })
        .fromTo(icon[2], 1, { opacity: 0 }, { opacity: 1 });
    }
  },
});

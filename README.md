# GitHub Tips, Bugtrack and How to Use this Repository.

## Using Git/GitHub for The First Time?
Check it out: [GIT basic usage with HT32SX](/git_basic_usage_with_ht32sx_repo.md)

<hr>

## Which branch to start a new project on?
All firmware examples can be found [**HERE**](https://github.com/htmicron/ht32sx/tree/master_2).

<hr>

## How to Clone this repository and all its branches?

```
git clone https://github.com/htmicron/ht32sx.git
cd ht32sx
git branch -r | grep -v '\->' | while read remote; do git branch --track "${remote#origin/}" "$remote"; done
git fetch --all
git pull --all
```

<hr>

## Bugtrack
If you want to issue a bug to our development team, please follow our [bugtrack guideline](How_to_bugtracker.md)

<hr>

## Tree view of the branches

```
 ├─ startPage
 │  ├─ documents
 |  |   ├─ Application Notes
 |  |   ├─ Evaluation Board Documentation
 |  |   ├─ Datasheet
 │  ├─ faq
 │  ├─ info
 |  |   ├─ How_to_bugtracker.md
 |  |   ├─ LICENSE
 |  |   ├─ README.md
 |  |   ├─ git_basic_usage_with_ht32sx_repo.md
 │  ├─ pcb_examples
 |  |   ├─ Test Board
 |  |   ├─ README.md
 │  ├─ master_2
 |  |   ├─ AT_Commands
 |  |   ├─ Bootloader_Refresh
 |  |   ├─ Generic_Push_Button
 |  |   ├─ HT32SX_P2P_Demo
 |  |   ├─ Monarch_MoveME
 |  |   ├─ Push_Button_RC2
 |  |   ├─ RTC_Wake_Up
 |  |   ├─ README.md
 │  ├─ binaries
 ├─ LICENSE
 └─ README.md
```

<hr>

## FAQ

Do you need help? Check our [FAQ](https://github.com/htmicron/ht32sx/tree/faq) or send an email to suppot_iot@htmicron.com.br. 

Enjoy! =)

---

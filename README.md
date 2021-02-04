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

You'll find here:
*  Examples - codes, aplication notes and design tips.

This map is from the master branch, yet it provides the general overview of the branches' structure.
```
 ├─ applications [application notes, examples]
 │  ├─ Evaluation_Board_Design_Reference
 │  ├─ Generic_Push_Button
 │  ├─ Monarch_Scan
 │  ├─ Monarch_MoveMe
 │  ├─ Push_Button
 │  ├─ antenna_design_reference
 │  ├─ bootloaderRefresh
 ├─ LICENSE
 └─ README.md
```

<hr>

## FAQ

Do you need help? Check our [FAQ](https://github.com/htmicron/ht32sx/tree/faq) or send an email to suppot_iot@htmicron.com.br. 

Enjoy! =)

---

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

## FAQ

Do you need help? Check our [FAQ](https://github.com/htmicron/ht32sx/tree/faq) or send an email to support_iot@htmicron.com.br. 

Enjoy! =)

---

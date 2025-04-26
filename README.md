# TrillionPass

The Password Manager

## Brief

The trillion pass is a command line password manager. It can generate random passwords and store them locally. The user need to set a master password to manage and encrypt all other passwords.

## Commands

Create

```sh
pass -c -name "name" -brief "the brief" -account "the name of the account"
# interactive create
pass -c -i
```

Find

```sh
pass -f name
```

Delete

```sh
pass -d name
```

List All Passwords

```sh
pass -l
```

Change Master Password

```sh
pass -mp -c
```
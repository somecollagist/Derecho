# Derecho File System Layout

```
root (/)
 |
 | --- dev            # Contains physical devices and nodes such as zero and random
 |
 | --- sys
 |      |
 |      | --- config  # Contains system-wide configuration files
 |      |
 |      | --- docs    # Contains system-wide documentation files
 |
 | --- tmp            # Storage for temporary data
 |
 | --- usr            # Contains user profiles
```
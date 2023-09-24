# 实验小作业

## 命题1

```assembly
Lemma ex1: forall A, ~~~ A -> ~ A.
Proof.
intro A.
intro H1.
intro H2.
apply H1.
intro H3.
apply H3.
apply H2.
Qed.
```

![image-20230604132800750](image/1-1.png)

![image-20230604132835022](image/1-2.png)

![image-20230604133426085](image/1-3.png)

![image-20230604133440162](image/1-4.png)

![image-20230604133457101](image/1-5.png)

![image-20230604133513962](image/1-6.png)

![image-20230604133529621](image/1-7.png)

![image-20230604133551871](C:/Users/wangc/AppData/Roaming/Typora/typora-user-images/image-20230604133551871.png)

## 命题2

```assembly
Lemma ex2: forall A B, A \/ B -> ~ (~ A /\ ~ B).
Proof.
intro A.
intro B.
intro H1.
intro H2.
destruct H1 as [H3 | H4].
destruct H2 as [H5 H6].
apply H5.
assumption.
destruct H2 as [H5 H6].
apply H6.
assumption.
Qed.
```

![image-20230604133735569](image/2-1.png)

![image-20230604133749693](image/2-2.png)

![image-20230604133807503](image/2-3.png)

![image-20230604133820975](image/2-4.png)

![image-20230604133836715](image/2-5.png)

![image-20230604133852652](image/2-6.png)

![image-20230604133906584](image/2-7.png)

![image-20230604133919960](image/2-8.png)

![image-20230604133935155](image/2-9.png)

![image-20230604133947557](image/2-10.png)

![image-20230604134001271](image/2-11.png)

![image-20230604134012694](image/2-12.png)

## 命题3

```assembly
Lemma ex3: forall T (P:T -> Prop), (~ exists x, P x) -> forall x, ~ P x.
Proof.
intro T.
intro P.
intro H1.
intro x.
intro H2.
apply H1.
exists x.
assumption.
Qed.
```

![image-20230604134136764](image/3-1.png)

![image-20230604134208344](image/3-2.png)

![image-20230604134222934](image/3-3.png)

![image-20230604134236775](image/3-4.png)

![image-20230604134249531](image/3-5.png)

![image-20230604134303037](image/3-6.png)

![image-20230604134318562](image/3-7.png)

![image-20230604134332023](image/3-8.png)

![image-20230604134349571](image/3-9.png)

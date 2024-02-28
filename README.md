# X-Lock
A secure XOR-based fuzzy extractor for resource constrained devices.

The Internet of Things rapid growth poses privacy and security challenges
for the traditional key storage methods.
Physical Unclonable Functions offer a potential solution but require
secure fuzzy extractors to ensure reliable replication.
This paper introduces X-Lock, a novel and secure computational
fuzzy extractor that addresses the limitations faced by traditional
solutions in resource-constrained IoT devices.
X-Lock offers a reusable and robust solution, effectively mitigating
the impacts of bias and correlation through its design.
Leveraging the preferred state of a noisy source, X-Lock encrypts a
random string of bits that can be later used as
seed to generate multiple secret keys.
To prove our claims, we provide a comprehensive theoretical
analysis, addressing both security considerations and implementation
details of the proposed model.
To evaluate the effectiveness and superiority of our proposal, we also
provide practical experiments and compare the results with existing approaches.
The experimental findings demonstrate the efficacy of our algorithm,
showing comparable memory cost ($\approx 2.4$ KB for storing
$5$ keys of $128$ bits) while being $3$ orders of magnitude faster with respect
to the state-of-the-art solution ($0.800$ ms against $15.51$ s).

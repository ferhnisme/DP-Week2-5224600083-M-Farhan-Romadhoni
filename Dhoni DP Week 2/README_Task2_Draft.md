# Task 2 - RuneForge Core Loop

## Step 1 - Core Loop
1. Player selects rune tiles from hand.
2. System evaluates the selected combination using the scoring system.
3. Reward or penalty is determined from the result.
4. Game state updates (score, casts, discard count, coins, and round state).
5. Optional shop phase happens after a successful round.
6. Repeat until the player clears all rounds or fails a target.

## Step 2 - Invariants
### 1. Which steps must NEVER change order?
The player selection phase must happen before evaluation because the system cannot score an empty action. Evaluation must happen before reward or penalty because the outcome depends on the calculated score. Reward or penalty must happen before state update because the updated state stores the result of the current action. The shop must only appear after a round is resolved, not before.

### 2. Which components must always exist?
- `RunSession` as the controller of the core loop
- `ScoringSystem` to evaluate rune combinations
- `RuneTile` as the data model for playable tiles
- State variables such as score, target, casts, discards, round, and coins

### 3. What would break if the order changes?
If the system evaluated before player input, there would be no valid tiles to score. If the reward phase happened before evaluation, the game would not know what reward or penalty to apply. If the state updated too early, score and round progression could become inconsistent. If the shop happened before round resolution, players could buy upgrades before earning them.

## Step 3 - Mutable Elements
1. **Target score per round** - this is mutable because it only changes difficulty tuning, not the loop structure.
2. **Shop catalog and prices** - the store content can be changed freely without changing the order of play.
3. **Modifier effects** - bonus values, multipliers, and special effects can be tuned independently from the loop.
4. **Rune pool composition** - rune names, powers, and elements can be adjusted to rebalance or add variety.

## Step 4 - Reflection (300-400 words)
The invariant structure of RuneForge is the repeated order of phases that defines one play cycle. First, the player selects rune tiles from the current hand. Second, the system evaluates the chosen combination through the scoring system. Third, the result becomes a reward or penalty because the score determines whether progress is made toward the target. Fourth, the game state is updated by changing total score, remaining casts, discard count, and other round variables. Fifth, once a round is successfully resolved, the player may enter the shop before the next round begins. This sequence is invariant because it provides the backbone of the game. If the order is broken, the system would lose the logical flow that connects player input, scoring, outcome, and progression.

The mutable parts are the mechanics and content placed inside that structure. Examples include the target score for each round, the values used by modifiers, the composition of the rune pool, and the list of purchasable upgrades in the shop. These parts are mutable because they can be changed for balancing, variety, or content expansion without changing the core order of phases. For example, adding a stronger multiplier or raising the round target does not require a new loop structure. The same loop still runs, only the content inside it changes.

If I wanted to add a new feature, the class that should change would depend on the feature. A new scoring rule would mostly affect `ScoringSystem`. A new purchasable upgrade would mainly affect `ModifierFactory`, `IModifier`, or `ShopSystem`. `RunSession` should change as little as possible because it represents the invariant structure. Its responsibility is controlling the order of phases, not storing all detailed mechanics.

If the loop order changed, multiple things would break. The system cannot evaluate before the player acts. Reward cannot be calculated before evaluation. State should not update before the result is known. Because of that, the phase order must remain stable even when features are added.
